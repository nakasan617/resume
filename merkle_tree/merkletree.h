
#ifndef MERKLETREE_H
#define MERKLETREE_H 

// defined here because of mutual dependence between bfs.h merkletree.h
// Node of Merkle Tree definition
struct node {
    //char *data;
    char *hash;
    int cur_height;
    int index;
    struct node *left;
    struct node *right;
};

typedef struct node node;

#include "bfs.h" 
#include <stdlib.h>
#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>

#define HASH_SIZE 256
#define BYTE_SIZE 8
#define HEX 4
#define PAGE_LENGTH 65536
#define BUFFER_MODE 1
#define FILE_MODE 0
#define RECOMPUTE 1
#define DONT -1
#define PRINT 4

// DEFINITIONS------------------------------------------------------------------

//Merkle Tree definition
typedef struct {
    int nb_nodes;
    int tree_height;
    size_t nb_of_leaves;
    size_t nb_of_pages;
    node *root;
} merkle_tree;

//MultiThread argument definition
typedef struct {
    node * current;
    int tree_height;
    int level;
} threaded_arg;

//SINGLE THREADED---------------------------------------------------------------

// Computes the Merkle tree of the datas.
int build_tree(merkle_tree *mt, char **datas);

// Computes the Merkle tree of the datas and times the operations. 
int build_tree_time(merkle_tree *mt, char **datas, double *times); 

// writes to the file in bfs way  
int mt_bfs_write(merkle_tree *mt, char path[]);

// writes to the file in dfs way 
int mt_dfs_write(merkle_tree *mt, char path[]);

// it is correspondend to mt_dfs_write 
int rebuild_from_file(char path [], merkle_tree * mt);

// returns the total number of nodes that are different
int compare_nodes(node * current, int cur_height, int tree_height, char ** parsed_file);

// it compares the hash, and if they are different, it recomputes
int compare_recompute_nodes(node * current, int cur_height, int tree_height, char ** parsed_file, int mode);

int get_tree_height(merkle_tree * mt);

size_t get_nb_leaves(merkle_tree * mt);
//------------------------------------------------------------------------------

//Frees Merkle tree.
void free_merkle(merkle_tree *mt);

#endif 
