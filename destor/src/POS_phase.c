#include "destor.h"
#include "jcr.h"
#include "backup.h"
#define POS_SIZE 128

#define getbit(from, which) ((from >> which) & 0x01) 

static pthread_t POS_t;

struct node {
	fingerprint fp; // fingerprint (this is NULL for the leaf nodes)
	struct node * next; // next sibling
	struct node * child; // first child
	int height; // current height
	int leaf; // if it is leaf, it is 1, ow, it is 0 
	unsigned int id; // to keep track (B+ tree id)
	int * versions; // this is NULL when the node is not leaf (hash map for which version that it is used)
};

typedef struct node node;

struct POS_tree {
	node * root;
	int height;
	int latest;
};
typedef struct POS_tree POS_tree;

extern POS_tree * ptree = NULL;

static void * POS_thread(void *);

void start_POS_phase() {
	POS_queue = sync_queue_new(POS_SIZE);
	pthread_create(&POS_t, NULL, POS_thread, NULL);
	//POS_thread(NULL);
}

void stop_POS_phase() {
	pthread_join(POS_t, NULL);
	NOTICE("POS phase stops successfully!");
}

node * create_sub_POS(void);

static void* POS_thread(void* arg) {

	node * root = create_sub_POS();
	ptree = malloc(sizeof(POS_tree));
	ptree -> root = root;
	ptree -> height = root -> height;
	ptree -> latest = 0;
	/* jcr.status = JCR_STATUS_DONE; */
	/* how are you going to reserve the POS tree after the computation? */
	return NULL;
}

void free_data_(void * data) {
	if(data != NULL)
		free(data);
	return;
}

inline void print_fp(fingerprint fp) {
	for(int i = 0; i < 20; i++) {
		printf("%x", fp[i]);
	}
	printf("\n");
}

int check_sha1(fingerprint fp) {
	// print_fp(fp);
	unsigned char b = fp[19];
	// printf("b: %x\n", b);
	/* returns 1 when you need to cut, returns 0 when you need to pass */
	if( !getbit(b, 0) && !getbit(b, 1) && !getbit(b, 2))
		return 1;
	return 0;
}

node * create_sub_POS(void) {
	node * first_leaf = malloc(sizeof(node));
	node * current = first_leaf;
	Queue * elements = queue_new();
	Queue * new_entries = queue_new();

	struct chunk * c = NULL;
	int count = 0;
	//int count = 1;
	while( (c = sync_queue_pop(hash_queue)) != NULL ) {
		/* if it was the start or the end, just continue */
		if(CHECK_CHUNK(c, CHUNK_FILE_START)) {
			sync_queue_push(POS_queue, c);
			continue;
		} else if ( CHECK_CHUNK(c, CHUNK_FILE_END)) {
			sync_queue_push(POS_queue,c);
			/* this current is in vain so I need to free it */
			free(current);
			continue;
		} 
		//printf("how many times %d\n", count++);
		/* otherwise initialize them */
		current->child = NULL;
		current->height = 0;
		current->leaf = 1;
		//current->c = c;
		memcpy(current->fp, c->fp, 20);
		current->id = 100 * (count++);
		current->versions = calloc(100, sizeof(int));
		current->versions[0] = 1;

		sync_queue_push(POS_queue, c);
		queue_push(new_entries, current);

		current -> next = malloc(sizeof(node));
		current = current->next;		
	} 
	sync_queue_term(POS_queue);
	
	/* this is the POS tree algorithm */
	/* you are going to hash until you hit the certain number */
	Queue * tmp = NULL;
	//count = 1;
	node * cur_parent = NULL;
	do {
		/* move all elements in new_entries to elements */
		/* pointer swappy */
		//printf("this is count %d\n", count++);
		//printf("before swappy\n elements: %d, new_entries: %d\n", queue_size(elements), queue_size(new_entries));
		tmp = elements; 
		elements = new_entries;
		new_entries = tmp;

		//printf("after swappy\n elements: %d, new_entries: %d\n", queue_size(elements), queue_size(new_entries));
		cur_parent = NULL;

		//int inside = 1;
		/* popping until you pop everything or you hit the certain hash */
		while((current = queue_pop(elements)) != NULL) {
			/*
				declare the parent here
				this part specifies what parent is
			*/
			//printf("this is inside: creating new parent %d\n", inside++);
			node * parent = malloc(sizeof(node));
			parent -> child = current;
			parent -> height = current->height + 1;
			parent -> leaf = 0;
			/* above line soon will be deleted */
			parent -> id = UINT_MAX;
			parent -> versions = NULL;
			
			if(cur_parent != NULL && parent -> height == cur_parent -> height) { 
				/* if this is not the case you are one level above. */
				cur_parent -> next = parent;
			} else {
				/* printf("/////////////////height is now %d//////////////////\n", parent->height); */
			}
			cur_parent = parent; /* update the cur_parent */
			cur_parent -> next = NULL; /* just to be sure. */
			
			/* since leaf has a hash already I am just going to compute the hash for parent */
			SHA_CTX ctx;
			SHA_CTX ctx0;
			SHA_Init(&ctx);
			static fingerprint tmp_fp; 

			SHA1_Update(&ctx, current->fp, 20);
			ctx0 = ctx;
			SHA1_Final(tmp_fp, &ctx0);
							
			if(check_sha1(tmp_fp)) { 
			/* check_sha1 checks whether the chunk needs to be cut according to the hash  */
			/* if this return true, finalize ctx, copy the hash, and continue */
				SHA1_Final(tmp_fp, &ctx);
				memcpy(cur_parent->fp,tmp_fp, 20);
				queue_push(new_entries, cur_parent);
				continue;
			}	

			/* if the hash of the first one is good then cut it off */

			/* if the hash of the first one is bad then go on to the next one */
			while((current = queue_pop(elements)) != NULL) {
				/*
					cut the lower level nodes when the following happens
						1. hash is the one to cut
				 		2. when there is no more lower level nodes
				*/
				/* below is 1 */
				SHA1_Update(&ctx, current->fp, 20);
				ctx0 = ctx;
				SHA_Final(tmp_fp, &ctx0);
				if(check_sha1(tmp_fp)) {
					SHA1_Final(tmp_fp, &ctx);
					memcpy(cur_parent->fp, tmp_fp, 20);
					queue_push(new_entries, cur_parent);
					break;
				}

				/* below is 2 */
				/* I did not break it because it will be cut off at next while loop */
				if(queue_size(elements) < 1) {
					SHA_Final(tmp_fp, &ctx);
					memcpy(cur_parent->fp, tmp_fp, 20);
					queue_push(new_entries, cur_parent);
				}
			}
		}  

	} while(queue_size(new_entries) > 1);

	node * root = queue_pop(new_entries);
	/* free the queue at last */ 
	queue_free(elements, free_data_);
	queue_free(new_entries, free_data_);

	/* we want to return the root node */
	return root;
}
