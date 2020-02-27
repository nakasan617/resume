#ifndef MAIN_H
#define MAIN_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>

#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/sha.h>

#define FP_SIZE 20
#define HASHMAP_SIZE 1<<10
#define BILLION 1000000000L
#define POS_ALL 2
#define POS_VERBOSE 1
#define POS_DEBUG 0

typedef unsigned char fingerprint[FP_SIZE];

struct key_value {
    void *key;
    void *value;
};
typedef struct key_value key_value;

struct hash_node {
    void *value;
    struct hash_node *next;
    fingerprint key;
};
typedef struct hash_node hash_node;

struct bucket {
    hash_node *top;
    int size;
};
typedef struct bucket bucket;

struct hashmap {
    int size;
    int capacity;
    struct bucket **buckets;
    void (*destroy_value) (void *);
    fingerprint *history;
};
typedef struct hashmap hashmap;

struct node {
    struct node **children;
    struct node *next;
    int height;
    int type;
    int num_children;
    fingerprint fp;
};
typedef struct node node;

struct hm_root {
    hashmap *hm;
    node *root;
};
typedef struct hm_root hm_root;

#include "node.h"
#include "queue.h"
#include "parse.h"
#include "hashmap.h"
#include "pos.h"

hm_root *iteration(char *path, hashmap *hm, int file_count);

#define DEBUG(verbosity, fmt, arg...) pos_log(verbosity, POS_DEBUG, fmt, ##arg);
#define VERBOSE(verbosity, fmt, arg...) pos_log(verbosity, POS_VERBOSE, fmt, ##arg);
#define ALL(verbosity, fmt, arg...) pos_log(verbosity, POS_ALL, fmt, ##arg);
void pos_log(int level, const char *fmt, ...);

#endif /* MAIN_H */
