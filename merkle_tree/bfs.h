#ifndef BFS_H
#define BFS_H

#include "merkletree.h"

struct bfs_node {
	node * mt_node;
	struct bfs_node * next;
};

typedef struct bfs_node bfs_node;

struct Queue {
	int length; 
	bfs_node * head; 
	bfs_node * tail;
};

typedef struct Queue Queue;

// this should return -1 if fail, 0 if success
Queue * bfs_init(node * root);

// this should enqueue the node at the tail. 
int bfs_enqueue(node * tail, Queue * queue);

// this should dequeque the node form the head. 
node * bfs_dequeue(Queue * queue);

// this prints out the queue to show how it is 
//void bfs_print(Queue * queue);

// returns 1 if it is empty, return 0 otherwise 
int bfs_isempty(Queue * queue);

// this should free bfs_node and Queue, but not node.
void bfs_free(Queue * queue);

#endif
