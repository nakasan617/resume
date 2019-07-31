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

#include "parse.h"

/*
 * Computes the size of a file given its pointer.
 * fp : pointer to the pointer of the file.
 * returns : size of the file.
 */

int get_file_size(FILE **fp) {
  fseek(*fp, 0L, SEEK_END);
  int size = ftell(*fp);
  fseek(*fp, 0L, SEEK_SET);

  return size;
}

int count_nb_nodes(int nb_of_leaves) {
	int to_add = nb_of_leaves;
	int rv = 0;
	while(to_add > 0) {
		rv += to_add;
		to_add = (int)to_add/2;
	}
	return rv;
}

int count_nb_of_leaves(int nb_of_pages) {
	int count = 0;
	while(nb_of_pages >= 1) {
		nb_of_pages = (int)(nb_of_pages/2);
		count++;
	}
	int rv = 1;
	for(int i = 0; i < count; i++) {
		rv = rv * 2;
	}

	return rv;
}
// this should return char ** parsed_file
char ** parse_file(FILE ** fp, int nb_of_pages, int nb_of_leaves, int file_size) {
    //Memory allocation
    char **parsed_file = (char **)calloc(nb_of_leaves, sizeof(char *) );


    for (int i = 0; i < nb_of_leaves; i++){
      parsed_file[i] = (char *)calloc(PAGE_LENGTH+1, sizeof(char));
    }

    //File parsing
    //parse_file(parsed_file, &file_string, nb_of_pages, tree_size, page_full);
    // this might cause a bug at the last page 
    for(int i = 0; i < nb_of_pages; i++) {
      if(file_size > PAGE_LENGTH) {
      	fread(parsed_file[i], PAGE_LENGTH, 1, *fp);
	file_size -= PAGE_LENGTH;
      }
      else {
        fread(parsed_file[i], file_size, 1, *fp);
      }
    }

    for(int i = nb_of_pages; i < nb_of_leaves; i++) {
      strcpy(parsed_file[i], " ");
    }
 
    return parsed_file;
}

/*
char ** m_parse_file(FILE ** fp, int nb_of_pages, int nb_of_leaves, int file_size, int num_threads) {
	char ** parsed_file = (char **)calloc(nb_of_leaves, sizeof(char *));

	omp_set_num_threads(num_threads);
	#pragma omp parallel for schedule(SCHEDULE_PARSE, BLOCK_SIZE_PARSE) default(none) shared(nb_of_leaves, parsed_file) private()
	for(int i = 0; i < nb_of_leaves; i++) {
		parsed_file[i] = (char *)calloc(PAGE_LENGTH + 1, sizeof(char));
	}

	#pragma omp parallel for schedule(SCHEDULE_PARSE, BLOCK_SIZE_PARSE) default(none) shared(nb_of_leaves, nb_of_pages, parsed_file, fp, file_size) private()	
	for(int i = 0; i < nb_of_leaves; i++) {
		if(i < nb_of_pages - 1) {
			// i is less than nb_of_pages - 1, then it should just read the whole thing 
			fread(parsed_file[i], PAGE_LENGTH, 1, (*fp) + (i*PAGE_LENGTH));
		} else if ( i == nb_of_pages - 1) {
			// if i is PAGE_LENGTH - 1, then it should read up to whatever you have left
			fread(parsed_file[i], file_size%PAGE_LENGTH, 1, (*fp) + (i*PAGE_LENGTH));
		} else {
			// if i is bigger than number of pages, it should copy the " "
			strcpy(parsed_file[i], " ");
		}
	}
	return parsed_file;
}
*/

void print_times(double * times) {
	fprintf(stdout, "Below are the times\n");
	int N = PRINT;
	for(int i = 0; i < N; i++) {
		if(i < N - 1) {
			fprintf(stdout, "%lf ", times[i]);
		} else {
			fprintf(stdout, "%lf\n", times[i]);
		}
	}
}

/*
 * Computes the Merkle tree given the parsed file.
 * parsed_file : pointers to pages of the file.
 *
 * result : string in which the resulting tree will be stored.
 * I am just storing the hashes in each node. 
 * returns : 0 if successful, 1 if not.
 */

int compute_merkle(FILE **fp, merkle_tree *mt, int nb_of_threads) {
    //File does not exist
    if (*fp == NULL) {
	perror("*fp is NULL\n");
        return -1;
    }

    int file_size = get_file_size(fp);

    //Compute array
    int nb_of_pages    = (int) (file_size / PAGE_LENGTH);
    int nb_of_leaves   = count_nb_of_leaves(nb_of_pages);  
    if(file_size % PAGE_LENGTH != 0) {
    	nb_of_pages++;
    }

    //parsing file
    char ** parsed_file = parse_file(fp, nb_of_pages, nb_of_leaves, file_size);

    //Assigning values for merkle_tree calculation
    int height = (int) log2(nb_of_leaves) + 1;
    mt->tree_height = height;
    mt->nb_nodes    = count_nb_nodes(nb_of_leaves); // this value is given in build_tree
    mt->nb_of_leaves = nb_of_leaves;
    mt->nb_of_pages = nb_of_pages;

    if(nb_of_threads != 1) {
	   if(m_build_tree(mt, parsed_file, nb_of_threads) == -1) {
		perror("build tree not successful\n");
		return -1;
	   }
    } else {
    	if(build_tree(mt, parsed_file) == -1) {
		perror("build tree not successful\n");
        	return -1;
    	}
    }

    //Freeing memory
    for(int i = 0; i < nb_of_leaves; i++){
	    free(parsed_file[i]);
    }
    free(parsed_file);

    return 0;
}

int compute_merkle_time(FILE **fp, merkle_tree *mt, int nb_of_threads, double * times) {
    //File does not exist
    if (*fp == NULL) {
	perror("*fp is NULL\n");
        return -1;
    }

    int file_size = get_file_size(fp);

    //Compute array
    int nb_of_pages    = (int) (file_size / PAGE_LENGTH);
    int nb_of_leaves   = count_nb_of_leaves(nb_of_pages);  
    if(file_size % PAGE_LENGTH != 0) {
    	nb_of_pages++;
    }
    printf("this is the number of pages %d\n", nb_of_pages);

    //parsing file
    clock_t start = clock();
    char ** parsed_file = parse_file(fp, nb_of_pages, nb_of_leaves, file_size);
    clock_t end = clock();
    double time_parse = (double)(end - start)/CLOCKS_PER_SEC;
    times[0] = time_parse; 

    //Assigning values for merkle_tree calculation
    int height = (int) log2(nb_of_leaves) + 1;
    mt->tree_height = height;
    mt->nb_nodes    = count_nb_nodes(nb_of_leaves); // this value is given in build_tree
    mt->nb_of_leaves = nb_of_leaves;
    mt->nb_of_pages = nb_of_pages;

    //printf("this is before build_tree\n"); 
    //print_times(times);
    start = clock();
    if(nb_of_threads != 1) {
	   if(m_build_tree(mt, parsed_file, nb_of_threads) == -1) {
		perror("build tree not successful\n");
		return -1;
	   }
    } else {
    	if(build_tree_time(mt, parsed_file, times) == -1) {
		perror("build tree not successful\n");
        	return -1;
    	}
    }
    end = clock();
    double time_build = (double)(end - start)/CLOCKS_PER_SEC;
    times[1] = time_build;

    //Freeing memory
    for(int i = 0; i < nb_of_leaves; i++){
	    free(parsed_file[i]);
    }
    free(parsed_file);

    return 0;
}


// this function should compare hash of the file and hash of the leaves of the merkletree, if there are differences, we will recompute the hashtree and returns the number of nodes recomputed. 
int compare_recompute(merkle_tree * mt, char file [], int mode) {
    FILE * fp = fopen(file, "r");
    //File does not exist
    if (fp == NULL) {
	perror("fp is NULL\n");
        return -1;
    }

    int file_size = get_file_size(&fp);

    //Compute array
    int nb_of_pages    = (int) (file_size / PAGE_LENGTH);
    int nb_of_leaves   = count_nb_of_leaves(nb_of_pages);  
    int nb_of_nodes    = count_nb_nodes(nb_of_leaves); // this value is given in build_tree
    if(file_size % PAGE_LENGTH != 0) {
    	nb_of_pages++;
    }

    // I am not sure if this part is correct
    //printf("this is nb_of_pages %d\n", nb_of_pages);
    //printf("this is nb_of_leaves %d\n", nb_of_leaves);

    char ** parsed_file = parse_file(&fp, nb_of_pages, nb_of_leaves, file_size);
    if(mt->nb_of_leaves != nb_of_leaves) {
	//fprintf(stderr, "number of leaves is different! Recomputing from scratch...\n");
	free_merkle(mt);
    	if (build_tree(mt, parsed_file) == -1) {
		perror("build tree not successful\n");
        	return -1;
    	}

	for(int i = 0; i < nb_of_leaves; i++) {
		free(parsed_file[i]);
	}
	free(parsed_file);
	fclose(fp);
	return 1;
    }
    if(mt->nb_nodes != nb_of_nodes) {
	perror("number of nodes is different\n");
	return -1;
    }

    // now you need to compare hash of each one. 
    int rv = -1;
    if((rv = compare_recompute_nodes(mt->root, mt->root->cur_height, mt->tree_height, parsed_file, mode)) == -1) {
	return -1;
    }
 
    for(int i = 0; i < nb_of_leaves; i++){
	    free(parsed_file[i]);
    }
    free(parsed_file);
    fclose(fp);

    return rv;
}

int print_node(Queue *queue, int cur_height, int tree_height) { 
	node * current = bfs_dequeue(queue);
	if(current->left) {
		if(bfs_enqueue(current->left, queue) == -1) {
			perror("error in enqueue\n");
			return -1; 
		}
	}
	if(current->right) { 
		 if(bfs_enqueue(current->right, queue) == -1) {
			 perror("error in enqueue\n");
			 return -1;
		 }
	}
	fprintf(stderr, "%s	", current->hash);
	if(current->index == (pow(2, cur_height) - 1)) {
		fprintf(stderr, "\n");
	}
	return 0;
}	

int print_tree(merkle_tree * mt) {
	int tree_height = mt->tree_height;
        Queue *queue = bfs_init(mt->root);	
	while(queue -> length > 0) {
		if(print_node(queue, 0, tree_height) == -1) {
			perror("failed to print a tree\n");
			return -1;
		}
	}
	bfs_free(queue);
	return 0; 
}

/*
 * Computes the Merkle tree given the parsed file. Multithreaded API.
 * fp : pointer to the pointer of the file.
 * merkle_tree : pointer to the tree that needs to be computed.
 * result : string in which the resulting tree will be stored.
 * returns : 0 if successful, 1 if not.
 * nb_threads : number of threads to invoke.
 */
/*
int m_compute_merkle(FILE **fp, merkle_tree *mt, char **result, int nb_threads) {
    //File does not exist
    if (*fp == NULL)
        return -1;

    int file_size = get_file_size(fp);

    //Reads file into file_string
    char *file_string = malloc(file_size + 1);
    fread(file_string, file_size, 1, *fp);

    //Compute array
    int page_full = is_full(file_size);
    int nb_of_pages = (int) file_size / PAGE_LENGTH;

    //If there is less pages than threads, we call the single-threaded method
    if (nb_of_pages < nb_threads) {
        return compute_merkle(fp, mt, result);
    }

    int tree_size = compute_tree_size(nb_of_pages + 1 - page_full);

    //Memory allocation
    char **parsed_file = calloc(tree_size, sizeof(char **) * sizeof(char *) * PAGE_LENGTH );
    *result            = malloc(sizeof(char *) * HASH_SIZE * 2 * tree_size);

    for (int i = 0; i < tree_size ; i++){
      *parsed_file = calloc(PAGE_LENGTH, sizeof(char *));
      parsed_file += PAGE_LENGTH;
    }
    parsed_file = parsed_file - tree_size*PAGE_LENGTH;

    //File parsing
    parse_file(parsed_file, &file_string, nb_of_pages, tree_size, page_full);

    //Assigning values for merkle_tree calculation
    int height = (int) log2(tree_size) + 1;
    mt->tree_height = height;
    mt->nb_nodes    = 0;
    mt->nb_of_leaves = tree_size;

    if (m_build_tree(mt, parsed_file, nb_threads) == -1) {
        //log_msg("%s\n", "ERROR : Tree could not build.");
        return -1;
    }

    //Stores result in a string
    tree_to_string(mt, *result);

    //Freeing memory
    free_merkle(mt);
    free_file(parsed_file,nb_of_pages, page_full);
    file_string -= ((nb_of_pages) * PAGE_LENGTH);
    free(parsed_file);
    free(file_string);

    return 0;
}
*/

