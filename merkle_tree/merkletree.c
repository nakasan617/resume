/*
 --------------------------------------------------------------------------------
 Copyright (C) 2018 SRI International
 This program is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------------------
 */

#include "merkletree.h"

//SINGLE THREADED---------------------------------------------------------------

int hash_tree(node * curr, int curr_height, int tree_height, char ** datas);
int hash_tree_time(node * curr, int curr_height, int tree_height, char ** datas, double * times);
/*
 * Computes the Merkle tree of the datas.
 * mt : pointer to the Merkle tree to be computed.
 * datas : pointers to pages to be copied in the tree.
 * returns : 0 if successful, 1 if not.
 */

int build_tree(merkle_tree *mt, char **datas) {

    //Setting values of Merkle Tree
    //mt->nb_nodes = (1 << (mt->tree_height));
    mt->root    = (node *)malloc(sizeof(node));
    mt->root->cur_height = 0;
    mt->root->index = 0;
    node * current = mt->root;

    if(hash_tree(current, 0, mt->tree_height, datas) == -1){
	    perror("a problem in hash_tree\n");
	    return -1;
    }

    return 0;
}

int build_tree_time(merkle_tree *mt, char **datas, double * times) {

    //Setting values of Merkle Tree
    mt->root    = (node *)malloc(sizeof(node));
    mt->root->cur_height = 0;
    mt->root->index = 0;
    node * current = mt->root;

    if(hash_tree_time(current, 0, mt->tree_height, datas, times) == -1){
	    perror("a problem in hash_tree\n");
	    return -1;
    }

    return 0;
}

int m_build_tree(merkle_tree *mt, char **datas, int level) {
	mt->root = (node *)malloc(sizeof(node));
	mt->root->cur_height = 0;
	mt->root->index = 0;
	node * current = mt->root;
	threaded_arg * arg = (threaded_arg *)malloc(sizeof(threaded_arg));
	arg->current = current; 
	arg->tree_height = mt->tree_height;
	arg->level = level;
	//int rv = *(m_hash_tree((void *)arg));
	int rv = 0;
	if(rv == -1) {
		perror("m_hash_tree failed\n"); 
		return -1;
	}
	return 0;
}

void sha256_double(char *string, char *string_two, char *outputBuffer) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, strlen(string));
	SHA256_Update(&sha256, string_two, strlen(string));
	SHA256_Final(hash, &sha256);
	int i = 0;
	for(i = 0; i < SHA256_DIGEST_LENGTH; i++) 
	{
		sprintf(outputBuffer + ( i* 2), "%02x", hash[i]);
	}
	outputBuffer[64] = '\0';
}

void sha256(char *string, char * outputBuffer)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = '\0';
}

int hash_tree(node * curr, int curr_height, int tree_height, char ** datas) {
	// if the node is not a leaf
	if(curr_height < tree_height - 1) {
		// I need to compute the hash, but use recursive function here. 
		curr->left = (node *)malloc(sizeof(node));
		curr->right = (node *)malloc(sizeof(node));
		curr->left->index = 2 * curr->index;
		curr->right->index = 2 * curr->index + 1;
		curr->left->cur_height = curr_height + 1;
		curr->right->cur_height = curr_height + 1;
		curr->hash = NULL;
		if(hash_tree(curr->left, curr_height + 1, tree_height, datas) == -1) {
			perror("a problem in hash_tree\n");
			return -1;
		}
		if(hash_tree(curr->right, curr_height + 1, tree_height, datas)== -1) {
			perror("a problem in hash_tree\n");
			return -1;
		}

		curr->hash = (char *)calloc(1, sizeof(char) * 65);
		sha256_double(curr->left->hash, curr->right->hash, curr->hash);
	}
	// if the node is a leaf
	else {
		curr->left = NULL;
		curr->right = NULL;
		curr->hash = NULL;
		curr->hash = (char *)calloc(1, sizeof(char) * 65);
		sha256(datas[curr->index], curr->hash);
	}
	return 0;
}

int hash_tree_time(node * curr, int curr_height, int tree_height, char ** datas, double * times) {
	// if the node is not a leaf
	if(curr_height < tree_height - 1) {
		// I need to compute the hash, but use recursive function here. 
		curr->left = (node *)malloc(sizeof(node));
		curr->right = (node *)malloc(sizeof(node));
		curr->left->index = 2 * curr->index;
		curr->right->index = 2 * curr->index + 1;
		curr->left->cur_height = curr_height + 1;
		curr->right->cur_height = curr_height + 1;
		curr->hash = NULL;
		if(hash_tree_time(curr->left, curr_height + 1, tree_height, datas, times) == -1) {
			perror("a problem in hash_tree\n");
			return -1;
		}
		if(hash_tree_time(curr->right, curr_height + 1, tree_height, datas, times)== -1) {
			perror("a problem in hash_tree\n");
			return -1;
		}

		curr->hash = (char *)calloc(1, sizeof(char) * 65);
		//printf("this is the time.2 before calculating %e\n", times[2]);
		clock_t start = clock();
		sha256_double(curr->left->hash, curr->right->hash, curr->hash);
		clock_t end = clock();
		//printf("this is the time.2 after calculating %e\n", times[2]);
		times[2] += (double)(end - start)/CLOCKS_PER_SEC;
	}
	// if the node is a leaf
	else {
		curr->left = NULL;
		curr->right = NULL;
		curr->hash = NULL;
		curr->hash = (char *)calloc(1, sizeof(char) * 65);
		//printf("this is the time.2 before calculating %e\n", times[2]);
		clock_t start = clock();
		sha256(datas[curr->index], curr->hash);
		clock_t end = clock();
		//printf("this is what we are adding %e\n", (double)(end - start)/CLOCKS_PER_SEC);
		times[3] += (double)(end - start)/CLOCKS_PER_SEC;
		times[2] += (double)(end - start)/CLOCKS_PER_SEC;
		//printf("this is the time.2 after calculating %e\n", times[2]);
	}
	return 0;
}

void * m_hash_tree(void * arg) {
	//threaded_arg * args = (threaded_arg *)arg;
	//node * current = args->current; 
	return NULL;
}
	
int mt_bfs_write(merkle_tree * mt, char path []) {
	FILE * fd = fopen(path, "w");
	if(fd == NULL) {
		perror("could not open the file\n");
		return -1;
	}

	// I first need to write number of leaves in the file
	fprintf(fd, "%d\n", mt->nb_nodes);
	//fprintf(stderr, "this is number of leaves %lu\n", mt->nb_of_leaves);
	Queue * queue = bfs_init(mt ->root);
	if(!queue) {
		perror("queue is null\n");
		return -1;
	}
	// if it is not empty, then we can dequeue the head
	while(queue->length > 0){
		node * head = bfs_dequeue(queue);
		// it is returning NULL here (dont know why) 
		fprintf(fd, "%s\n", head->hash);
		// if there are children, enqueue them
		if(head->left) {
			if(bfs_enqueue(head->left, queue)==-1) {
				perror("left enqueue not successful\n");
				return -1;
			}
		}
		if(head->right) {		
			if(bfs_enqueue(head->right, queue) == -1) {
				perror("right enqueue not successful\n");
				return -1;
			}
		}
	}
	bfs_free(queue);
	fclose(fd);

	return 0;
}

int nodes_dfs_write(node * curr, int cur_height, int height, FILE *fd);

int mt_dfs_write(merkle_tree * mt, char path []) { 
	FILE * fd = fopen(path, "w");
	if(fd == NULL) { 
		perror("could not open the file\n");
		return -1;
	}

	fprintf(fd, "%d\n", mt->nb_nodes);
	if(nodes_dfs_write(mt->root, 0, mt->tree_height, fd) == -1) {
		perror("dfs write failed\n");
		return -1;
	}

	fclose(fd);
	return 0;
}

int nodes_dfs_write(node * curr, int cur_height, int height, FILE *fd) {
	if(curr == NULL) 
		return -1;

	// this is the key operation
	fprintf(fd, "%s\n", curr -> hash);

	// we are at the leaf already
	if(cur_height == height - 1 ) {
		return 0;
	}
	// else we need to recursively call the functions
	if(nodes_dfs_write(curr -> left, cur_height + 1, height, fd) == -1) 
		return -1;
	if(nodes_dfs_write(curr -> right, cur_height + 1, height, fd) == -1) 
		return -1;
	return 0;

}

int rebuild_node(FILE * fp, node * current, int tree_height);
int rebuild_from_file(char path [], merkle_tree * mt) {
	if(!mt) {
		perror("Memory must be allocated for merkle tree!\n");
		return -1;
	}

	FILE * fp = fopen(path, "r");
	// since first line is the number of nodes, we read the first line. 
	if(!fp) {
		perror("Could not open the file\n");
		return -1;
	}

	char * buf = NULL;
	size_t n = 0;
	getline(&buf, &n, fp);
	int nb_of_nodes = atoi(buf);
	free(buf);
	mt->tree_height = log2(nb_of_nodes + 1);
	mt->nb_nodes = nb_of_nodes; 
	if((nb_of_nodes + 1) % 2 != 0) {
		perror("invalid nb_of_nodes!\n");
		return -1;
	}

	mt->nb_of_leaves = (nb_of_nodes + 1)/2;
	mt->root = (node *)malloc(sizeof(node));
	mt->root->cur_height = 0;
	mt->root->index = 0;
	//mt->root->data = NULL;
	mt->root->hash = NULL;
	mt->root->left = NULL;
	mt->root->right = NULL;

	if(rebuild_node(fp, mt->root, mt->tree_height) == -1) {
		perror("problem in rebuilding\n");
		return -1;
	}
	
	fclose(fp);
	return 0;
}

int rebuild_node(FILE * fp, node * current, int tree_height) {
	if(!fp) {
		perror("memory is not allocated for fp\n");
		return -1;
	}
	if(!current) {
		perror("memory is not allocated for node\n");
		return -1;
	}

	// here is the key operation. 
	char * buf = NULL;
	size_t n = 0;
	getline(&buf, &n, fp); 
	// I need to take out \n at the end; 
	char * to_cpy = buf; 
	current->hash = (char *)calloc(1, sizeof(char) * (64 + 1));
	strcpy(current->hash, strtok(to_cpy, "\n"));
	free(buf);

	// if it is at the leaf, just return
	if(current->cur_height == tree_height - 1) {
		return 0;
	}
	// if not recursively call the function	
	else {
		// allocate memory for the children 
		current->left = (node *)malloc(sizeof(node));
		current->right = (node *)malloc(sizeof(node));
		current->left->cur_height = current->cur_height + 1; 
		current->right->cur_height = current->cur_height + 1; 
		current->left->index = 2 * current->index; 
		current->right->index = 2 * current->index + 1;

		// make sure those that are not suppposed to null is null
		current->left->hash = NULL;
		current->right->hash = NULL;
		current->left->left = NULL;
		current->left->right = NULL;
		current->right->left = NULL;
		current->right->right = NULL;

		// Then (finally!) call the functions
		if(rebuild_node(fp, current->left, tree_height)== -1) {
			return -1;
		}
		if(rebuild_node(fp, current->right, tree_height) == -1) {
			return -1; 
		}
	}
	return 0;
}	

/*
int compare_recompute_nodes(node * current, int cur_height, int tree_height, char ** parsed_file) {
	return 0;
}
*/

// returns how many nodes have been changed
int compare_recompute_nodes(node * current, int cur_height, int tree_height, char ** parsed_file, int mode) {
	// if it is not a leaf 
	if(cur_height+1 < tree_height) {
		if(!current->left) {
			perror("left child where it is supposed to be there, is not there\n");
			return -1;
		}
		if(!current->right) {
			perror("right child where it is supposed to be there, is not there\n");
			return -1;
		}

		// below is the recursive function
		int rv_left = compare_recompute_nodes(current->left, cur_height + 1, tree_height, parsed_file, mode);
		int rv_right = compare_recompute_nodes(current ->right, cur_height + 1, tree_height, parsed_file, mode);

		if(rv_left == -1 || rv_right == -1) // this means error happened 
			return -1;
		else if(rv_left == 0 && rv_right == 0) // this means so far the hashes are the same
			return 0;
		else {
			if(mode == RECOMPUTE) {
				char * left = malloc(65);
				strcpy(left, current->left->hash);
				char * right = malloc(65);
				strcpy(right, current->right->hash);
				sha256_double(left, right, current->hash);
				free(left);
				free(right);
			}
			// here I need to recompute the hash
			// printf("I was dif not leaf\n");

			return rv_left + rv_right + 1;
		}
	}
	// if it is a leaf
	else {
		// see what the hash is. 
		if(!current->hash) {
			perror("memory not allocated for the leaves hash\n");
			return -1;
		}
		char * hash_string = calloc(1, sizeof(char) * 65);
		char * data = parsed_file[current->index];
		sha256(data, hash_string);

		if(!strcmp(current->hash, hash_string)) {
			free(hash_string);
			return 0;
		}
		else {
			if(mode == RECOMPUTE) {
				strcpy(current->hash, hash_string);
			}
			assert(current->hash != NULL);
			free(hash_string);

			// printf("I was dif\n");
			return 1;
		}
	}
}

int get_tree_height(merkle_tree * mt) {
	if(!mt) {
		return -1;
	}
	return mt->tree_height;
}

size_t get_nb_leaves(merkle_tree * mt) {
	if(!mt) {
		return -1;
 	}
	return mt->nb_of_leaves; 
}
//------------------------------------------------------------------------------

/*
 * Frees Merkle tree.
 * mt : pointer to Merkle tree.
 * returns : NULL.
 */
void free_nodes(node * node) {
    if(node->left != NULL) {
    	free_nodes(node->left);
    }
    if(node->right != NULL) {
    	free_nodes(node->right);
    }
    if(node) {
	if(node->hash) {
		free(node->hash);
	}
        free(node);
    }
    return;
}

// note that this does not free merkle_tree itself!
void free_merkle(merkle_tree *mt) {
    free_nodes(mt->root);
    return;
}


