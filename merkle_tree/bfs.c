#include "bfs.h" 
#include <stdlib.h>

Queue * bfs_init(node * root) {
	// this part is allocating the memory for Queue
	Queue * queue = (Queue *)malloc(sizeof(Queue));

	// this part is initializing the root as a bfs_node 
	bfs_node * bfs_root = (bfs_node *)malloc(sizeof(bfs_node));
	bfs_root -> mt_node = root;
	bfs_root -> next = NULL;
	
	// this part is initializing the Queue
	queue -> length = 1;
	queue -> head = bfs_root;
	queue -> tail = bfs_root;
	return queue;
}

int bfs_enqueue(node * tail, Queue * queue) {
	// we first need to sugar coat tail as bfs_node
	bfs_node * bfs_tail = (bfs_node *)malloc(sizeof(bfs_node));
	bfs_tail -> mt_node = tail;
	bfs_tail -> next = NULL;

	// let's enqueue
	// making the current tail's next as new tail.
	//fprintf(stderr, "%lu\n", bfs_tail);
	//fprintf(stderr, "%lu\n", sizeof(queue->tail->next));
	if(queue -> length == 0) {
		queue -> head = bfs_tail;
		queue -> tail = bfs_tail;
	}
	else {
		queue -> tail -> next = bfs_tail;
		// new_tail taking the spot of the old tail. 
		queue -> tail = bfs_tail;
	}
	
	// add one to the lengt
	queue -> length += 1;
	
	return 0;
}

node * bfs_dequeue(Queue * queue) {
	
	if(queue -> length < 1) 
	{
		perror("queue is empty");
		return NULL;
	}
	
	node * return_node = queue -> head -> mt_node; 

	// replace old head with new head
	bfs_node * tmp = queue -> head;
	if(queue -> head -> next) {
		queue -> head = queue -> head -> next;
	}
	free(tmp);
	queue -> length -= 1;
	
	return return_node;
}
/*
int bfs_isempty(Queue * queue) {
	if(queue -> head)
		return 0;
	return 1;
}
*/
void bfs_free(Queue * queue) {
	if(queue -> length > 0) {
		while(queue -> head) {
			bfs_node * tmp = queue -> head; 
			queue -> head = queue -> head -> next; 
			free(tmp);
		}
	}
	free(queue);
}
