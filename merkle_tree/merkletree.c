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

void print_times_merkle(double * times) {
    fprintf(stdout, "Below are the times\n");
    int N = PRINT; 
    for(int i = 0; i < N; i++) {
        if(i < N -1) {
            fprintf(stdout, "%lf ", times[i]);
        } else {
            fprintf(stdout, "%lf\n", times[i]);
        }
    }
}
    
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
    //printf("this is before hashing\n");
    //print_times_merkle(times);

    if(hash_tree_time(current, 0, mt->tree_height, datas, times) == -1){
	    perror("a problem in hash_tree\n");
	    return -1;
    }
    //printf("this is after hashing\n");
    //print_times_merkle(times);

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
//MULTI THREADED----------------------------------------------------------------

//int m_build_tree(merkle_tree *mt, char **datas, int nb_of_threads) {
//	return 0;
//}
/*
 * Fills the data of leaves with a parsed file.
 * mt : pointer to the Merkle tree to be modified.
 * datas : parsed file to copy into the tree's data.
 * returns : 0 if successful, 1 if not.
 */
/*
int set_tree_datas(merkle_tree *mt, char **datas) {

    int total_size = 0;
    //Setting values of Merkle Tree
    int leaf_start_index = (1 << (mt->tree_height - 1));
    mt->nb_nodes         = (1 << (mt->tree_height));
    mt->nodes            = malloc(sizeof(node) * (mt->nb_nodes + 1) * 2 * PAGE_LENGTH * HASH_SIZE);
    //Setting tree datas
    for (int i = leaf_start_index; i < mt->nb_nodes; i++) {
        mt->nodes[i].data = malloc(sizeof(char *) * 2 * PAGE_LENGTH);

        //If there is a problem with data, send error
        if (*(datas + total_size) == NULL) {
            log_msg("%s\n", "ERROR in set_tree_datas: NULL somewhere in data");
            return -1;
        }

        strlcpy(mt->nodes[i].data, *(datas + total_size), 1 + strlen(*(datas + total_size)));
        total_size       += strlen(*(datas + total_size));
        mt->nodes[i].hash = NULL;
    }

    return 0;

}
*/
/*
 * Computes the Merkle tree of the datas. Multithreaded API.
 * mt : pointer to the Merkle tree to be computed.
 * datas : pointers to pages to be copied in the tree.
 * nb_of_threads : number of threads to be invoked.
 * returns : 0 if successful, 1 if not.
 */
/*
int m_build_tree(merkle_tree *mt, char **datas, int nb_of_threads) {

    int leaf_start_index = (1 << (mt->tree_height - 1));

    if (set_tree_datas(mt, datas) == -1)
        return -1;

    int nb_leaves_per_thread = (int) leaf_start_index / nb_of_threads ;
    int start_index;
    pthread_t threads[nb_of_threads];
    threaded_arg t_arg[nb_of_threads];

    int new_start_index = leaf_start_index / (1 << ((int)log2(nb_leaves_per_thread) - 1));

    for (int i = 0 ; i < nb_of_threads - 1 ; i ++) {
        //creates thread that computes the hashes
        start_index          = leaf_start_index + (i * nb_leaves_per_thread);
        t_arg[i].mt          = mt;
        t_arg[i].start_index = start_index;
        t_arg[i].nb_leaves   = nb_leaves_per_thread;

        if (pthread_create(&threads[i], NULL, &m_hash_nodes, (void *)&t_arg[i]) != 0)
            return -1;

    }

    //The last thread potentially has to compute more than nb_leaves_per_thread
    start_index                          = leaf_start_index + (nb_of_threads - 1) * nb_leaves_per_thread ;
    t_arg[nb_of_threads - 1].mt          = mt ;
    t_arg[nb_of_threads - 1].start_index = start_index;
    t_arg[nb_of_threads - 1].nb_leaves   = nb_leaves_per_thread + (leaf_start_index % nb_of_threads);

    if (pthread_create(&threads[nb_of_threads - 1], NULL, &m_hash_nodes, (void *)&t_arg[nb_of_threads - 1]) != 0)
        return -1;

    //Waits for every thread to be done
    for (int i = 0 ; i < nb_of_threads ; i ++) {
        pthread_join(threads[i], NULL);
    }

    //Hashing root nodes
    for (int i = new_start_index ; i > 0; i--) {
        mt->nodes[i].hash = NULL;
        if (hash_node(mt, i) == -1)
            return -1;
    }

    return 0;
}
*/
/*
 * Hashes the data of several nodes. Multithreaded API.
 * returns : NULL.
 */
/*
void *m_hash_nodes(void *arg) {

    //Getting arguments through struct
    threaded_arg *t_arg = (threaded_arg *)arg;
    int start_index     = t_arg->start_index;
    int nb_leaves       = t_arg->nb_leaves;
    merkle_tree *mt     = t_arg->mt;

    int nb_level = log2(nb_leaves);

    for (int i = 0 ; i < nb_level ; i ++) {
        for (int j = start_index ; j < start_index + nb_leaves ; j++ ) {
              if (hash_node(mt, j) == -1)
                  log_msg("%s %d\n", "Problem with node number", j );
        }
        start_index = start_index / 2 ;
        nb_leaves   = nb_leaves / 2 ;
    }

    return NULL;
}
*/
//------------------------------------------------------------------------------

// STRING FORMATTING------------------------------------------------------------

/*
 * Prints hash of node.
 * mt : pointer to Merkle tree.
 * i : index of node to print.
 * returns : NULL.
 */
/*
void print_hash(merkle_tree *mt, int i) {
    char *hash = mt->nodes[i].hash;
    printf("%s", hash );
}
*/
/*
 * Prints whole tree.
 * mt : pointer to Merkle tree.
 * returns : NULL.
 */
/*
void print_tree(merkle_tree *mt) {
    for (int i = 1; i < mt->nb_nodes ; i++ ) {
        printf("%d\n", i);
        print_hash(mt, i);
        printf("\n");
    }
}
*/
/*
 * Formats Merkle tree to string.
 * mt : pointer to Merkle tree.
 * tree : string to which the tree will be copied.
 * returns : NULL.
 */
/*
void tree_to_string(merkle_tree *mt, char tree[]) {

    char *tree_string = malloc(HEX * HASH_SIZE * mt->nb_nodes);
    char *hash;
    char number_string[10];
    tree_string[0] = '\0';

    for (int i = 1 ; i < mt->nb_nodes ; i++ ) {
        sprintf(number_string, "%d", i);
        strcat(tree_string, number_string);
        strcat(tree_string, ":");
        hash = mt->nodes[i].hash;
        strcat(tree_string, hash);
        if (i < mt->nb_nodes-1)
            strcat(tree_string, ",");
    }
    strcpy(tree, tree_string);
    free(tree_string);

}
*/
/*
 * Splits string with delimitation char.
 * a_str : string to be split.
 * a_delim : delimitation string.
 * returns : pointer to splitted strings.
 */
/*
char** str_split(char* a_str, const char a_delim) {

    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // Count how many elements will be extracted. 
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for trailing token. 
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}
*/
/*
 * Fills tree with hashes in string.
 * mt : pointer to Merkle tree to be filled.
 * tree_string : string representing a tree.
 * returns : NULL.
 */
/*
void string_to_tree(merkle_tree *mt, char *tree_string) {

    char **tokens;
    char **thing;
    tokens = str_split(tree_string, ',');
    if (tokens)
    {
        int nb_nodes;
        for (nb_nodes = 0; *(tokens + nb_nodes); nb_nodes++) {}

        mt->nb_nodes    = nb_nodes + 1;
        mt->tree_height = (int) log2(nb_nodes + 1);
        mt->nodes       = malloc(sizeof(node) * (mt->nb_nodes + 1) * 2 * PAGE_LENGTH * HASH_SIZE);


        for (int i = 0; *(tokens + i); i++)
        {
            mt->nodes[i + 1].hash = malloc(sizeof(char *) * 2 * HASH_SIZE / BYTE_SIZE);
            thing                 = str_split(*(tokens + i), ':');
            char *hash_string     = *(thing + 1);
            strcpy(mt->nodes[i + 1].hash, hash_string);
        }
        free(tokens);
    }
}
*/
//------------------------------------------------------------------------------

// COMPARISONS AND DATA CHANGES-------------------------------------------------

/*
 * Changes pages of tree and recomputes hashes.
 * mt : pointer to Merkle tree to be filled.
 * indexes : indexes of pages that need to be changed.
 * datas : pointer to strings of new pages.
 * number : number of pages that need to be changed.
 * mode : file or buffer mode.
 * returns : 0 if successful, 1 if not.
 */
/*
int change_and_rebuild(merkle_tree *mt, int indexes[], char **datas, int number, int mode) {

    int index;
    int data_size = 0;
    if (mode == FILE_MODE) {
        for (int i = 0; i < number; i ++) {
            index                 = indexes[i];
            mt->nodes[index].data = *(datas + data_size);
            data_size            += strlen(*(datas + data_size));
        }

        int k  = number;
        index = indexes[0];

        for (int j = 0 ; j < k ; j ++) {
          if (hash_node(mt, index) == -1)
              return -1;
          index++;
        }
        index = index - k;
        index = index / 2;
        k     = (k / 2) + 1;

        int tree_height = mt->tree_height;

        for (int i = 0 ; i < tree_height ; i ++) {
            for (int j = 0 ; j < k ; j ++) {
              mt->nodes[index].hash = malloc(sizeof(char *)* strlen("deprecated"));
              strcpy(mt->nodes[index].hash, "deprecated");
              index++;
            }
            index = index - k;
            index = index / 2;
            k     = (k / 2) + 1;
        }
    }

    else {
      for (int i = 0; i < number; i ++) {
          index                 = indexes[i];
          mt->nodes[index].data = malloc(sizeof(char *) * 2 * PAGE_LENGTH);
          strlcpy(mt->nodes[index].data, *(datas + data_size), 1 + strlen(*(datas + data_size)));
          data_size            += strlen(*(datas + data_size));
      }

      mt->nodes[index].hash = NULL;
      if (hash_node(mt, index) == -1)
        return -1;
      index = index / 2;

      while (index !=0){
          mt->nodes[index].hash = malloc(sizeof(char *)* strlen("deprecated"));
          strcpy(mt->nodes[index].hash, "deprecated");
          index = index / 2;
      }
    }

    return 0;
}
*/
/*
 * Prints differences between two Merkle trees.
 * mt_a : pointer to Merkle tree A.
 * mt_b : pointer to Merkle tree B.
 * index : index to compare (recursively).
 * returns : NULL.
 */
/*
void compare_trees(merkle_tree *mt_a, merkle_tree *mt_b, int index) {

    //To compare whole tree, call compare_trees(a,b, 1)
    if (strcmp(mt_a->nodes[index].hash, mt_b->nodes[index].hash)) {
        printf("%s", "Difference on node number ");
        printf("%d\n", index);

        //If there is a difference and the node is not a leaf, check nodes below.
        if (index < (1 << (mt_a->tree_height - 1)) ) {
            compare_trees(mt_a, mt_b, 2*index);
            compare_trees(mt_a, mt_b, 2*index+1);
        }

    }
}
*/
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


