#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h> 
#include <omp.h>
#include <time.h>
#include "merkletree.h"

#define MAX_TREE_SIZE 64000000
#define SCHEDULE_PARSE guided 
#define BLOCK_SIZE_PARSE 1

//Computes closest power of two of the input.
int compute_tree_size(int index);

int get_file_size(FILE ** fp);

int count_nb_of_leaves(int nb_of_pages);

char ** parse_file(FILE ** fp, int nb_of_pages, int nb_of_leaves, int file_size);

char ** m_parse_file(FILE ** fp, int nb_of_pages, int nb_of_leaves, int file_size, int num_threads);

//Computes the Merkle tree given the parsed file.
int compute_merkle(FILE **fp, merkle_tree *mt, int nb_of_threads);

int compute_merkle_time(FILE **fp, merkle_tree *mt, int nb_of_threads, double * times);

// it compares and recomputes the nodes, if you put RECOMPUTE at mode, it recomputes. If you put DONT it just compares the nodes and counts how many are different.
// returns number of nodes that are recomputed, return -1 in case of an error.
int compare_recompute(merkle_tree * mt, char file [], int mode);

//Computes the Merkle tree given the parsed file. Multithreaded API.
int m_compute_merkle(FILE **fp, merkle_tree *mt, char **result, int nb_threads);

//Updates the Merkle tree after a change has been made to the file.
int pages_in_need(int size, int offset, merkle_tree *mt, FILE **fp, char **result);

//If a write changes a full page, updates the Merkle tree given a buffer
//without opening the file.
int quick_change(int size, int offset, char **buf, merkle_tree *mt, char ** result);

//Computes the hashes of the nodes that are not leaves.
int root_calculation(merkle_tree *mt, char **result);
