//#include <CUnit/Basic.h> 

#include "../parse.h"
#include "../merkletree.h"

#include <assert.h>
//Tests the function of parse.c. Prints the Merkle Tree of parse.h file.

// this is the test case for the large file.
int test_case1(void) {
    //fprintf(stderr, "this is test case 1\n");

    char path [] = "./example/largefile.txt";
    FILE *fp = fopen(path, "r");
    merkle_tree mt_a;

    if(compute_merkle(&fp, &mt_a, 1) == -1) {
	    //perror("compute merkle failed\n");
	    return -1;
    }

    char path_write [] = "./example/metadatas/dfs.txt";
    if(mt_dfs_write(&mt_a, path_write) == -1) {
	    //perror("mt_dfs_write failed\n"); 
	    return -1;
    }
    free_merkle(&mt_a);
    fclose(fp);
    
    char path2 [] = "./example/metadatas/dfs.txt";
    merkle_tree * mt = (merkle_tree *)malloc(sizeof(merkle_tree));
    if(rebuild_from_file(path2, mt) == -1) {
	    //perror("rebuilding the merkletree failed\n");
	    return -1;
    }
    
    //fprintf(stderr, "I am printing tree here\n");
    //print_tree(mt);
    
     
    char file [] = "./example/largefile.txt";
    int rv = -1;
    /*
    if((rv = compare_recompute(mt, file, RECOMPUTE)) == -1) {
	    //perror("compare and recompute failed\n");
    }
    else if(rv == 0) {
	    fprintf(stderr, "the nodes were the same\n");
    }
    else {
	    fprintf(stderr, "%d nodes were different and it was recomputed\n", rv);
    }
    */
    if((rv = compare_recompute(mt, file, RECOMPUTE)) != 0) {
            return -1;
    }

    char path_same [] = "./example/metadatas/dfs_same.txt";
    if(mt_dfs_write(mt, path_same) == -1) {
	    //fprintf(stderr, "the mt_dfs_write has failed\n");
	    return -1;
    }

    // this one has such change of a single letter. 
    char file_three [] = "./example/largefile3.txt";
    /*
    if((rv = compare_recompute(mt, file_three, RECOMPUTE)) == -1) {
	    fprintf(stderr, "compare and recompute failed\n");
    }
    else if(rv == 0) {
	    fprintf(stderr, "the nodes were the same\n");
    }
    else {
	    fprintf(stderr, "%d nodes were different and recomputed\n", rv);
    }
    */
    rv = compare_recompute(mt, file_three, RECOMPUTE);
    if(rv <= 0) {
            return -1;
    }

    char path_diff [] = "./example/metadatas/dfs_small_change.txt";
    if(mt_dfs_write(mt, path_diff) == -1) {
	    //fprintf(stderr, "the mt_dfs_write has failed\n");
	    return -1;
    }
    
    // the file has been changed a lot.
    char file_two [] = "./example/largefile2.txt";
    // should return number of leaves that are changes 
    /*
    if((rv = compare_recompute(mt, file_two, RECOMPUTE)) == -1) {
	    fprintf(stderr, "compare and recompute failed\n");
	    return -1;
    }
    else if(rv == 0) {
	    fprintf(stderr, "the nodes were the same\n");
    }
    else {
	    fprintf(stderr, "%d nodes were different and it was recomputed\n", rv);
    }
    */
    if((rv = compare_recompute(mt, file_two, RECOMPUTE)) != 31) {
            return -1;
    }

    char path_dif [] = "./example/metadatas/dfs_large_change.txt";
    if(mt_dfs_write(mt, path_dif) == -1) {
	    //fprintf(stderr, "the mt_dfs_write has failed\n");
	    return -1;
    }
 
    free_merkle(mt);
    free(mt);

    //fprintf(stderr, "\n");

    return 0;
}

int test_case2() {
    //fprintf(stderr, "this is test case 2\n");
    char path [] = "./example/hello.txt";
    FILE *fp = fopen(path, "r");
    merkle_tree mt_a;

    if(compute_merkle(&fp, &mt_a, 1) == -1) {
	    //perror("compute merkle failed\n");
	    return -1;
    }

    char path_write [] = "./example/metadatas/dfs.txt";
    if(mt_dfs_write(&mt_a, path_write) == -1) {
	    //perror("mt_dfs_write failed\n"); 
	    return -1;
    }
    free_merkle(&mt_a);
    fclose(fp);
    
    char path2 [] = "./example/metadatas/dfs.txt";
    merkle_tree * mt = (merkle_tree *)malloc(sizeof(merkle_tree));
    if(rebuild_from_file(path2, mt) == -1) {
	    //perror("rebuilding the merkletree failed\n");
	    return -1;
    }
    
    //fprintf(stderr, "I am printing tree here\n");
    //print_tree(mt);
    
     
    char file [] = "./example/hello.txt";
    int rv = -1;
    /*
    if((rv = compare_recompute(mt, file, RECOMPUTE)) == -1) {
	    perror("compare and recompute failed\n");
    }
    else if(rv == 0) {
	    fprintf(stderr, "the nodes were the same\n");
    }
    else if(rv == 1) {
	    fprintf(stderr, "the nodes hash were different and recomputed\n");
    }
    */
    if((rv = compare_recompute(mt, file, RECOMPUTE)) != 0) {
            return -1;
    }

    char path_same [] = "./example/metadatas/dfs_same.txt";
    if(mt_dfs_write(mt, path_same) == -1) {
	    //fprintf(stderr, "the mt_dfs_write has failed\n");
            return -1;
    }

    // this one has such change of a single letter. 
    char file_three [] = "./example/hello_small_change.txt";
    /*
    if((rv = compare_recompute(mt, file_three, RECOMPUTE)) == -1) {
	    fprintf(stderr, "compare and recompute failed\n");
    }
    else if(rv == 0) {
	    fprintf(stderr, "the nodes were the same\n");
    }
    else if(rv == 1) {
	    fprintf(stderr, "the nodes hash were different and recomputed\n");
    }
    else {
	    fprintf(stderr, "rv was %d\n", rv);
	    return -1;
    }
    */
    if((rv = compare_recompute(mt, file_three, RECOMPUTE)) != 1) {
            return -1;
    }

    char path_diff [] = "./example/metadatas/dfs_small_change.txt";
    if(mt_dfs_write(mt, path_diff) == -1) {
	    //fprintf(stderr, "the mt_dfs_write has failed\n");
	    return -1;
    }
   

    free_merkle(mt);
    free(mt);

    //fprintf(stderr, "\n");
    return 0;
}       

int test_case3() {
    //fprintf(stderr, "this is test case 3\n");
    char path [] = "./example/hello.txt";
    FILE *fp = fopen(path, "r");
    merkle_tree mt_a;

    if(compute_merkle(&fp, &mt_a, 1) == -1) {
	    perror("compute merkle failed\n");
	    return -1;
    }

    char path_write [] = "./example/metadatas/dfs.txt";
    if(mt_dfs_write(&mt_a, path_write) == -1) {
	    perror("mt_dfs_write failed\n"); 
	    return -1;
    }

    free_merkle(&mt_a);
    fclose(fp);
    
    char path2 [] = "./example/metadatas/dfs.txt";
    merkle_tree * mt = (merkle_tree *)malloc(sizeof(merkle_tree));
    if(rebuild_from_file(path2, mt) == -1) {
	    perror("rebuilding the merkletree failed\n");
	    return -1;
    }

    char file [] = "./example/largefile.txt";
    int rv = -1;
    if((rv = compare_recompute(mt, file, RECOMPUTE)) == -1) {
	    perror("compare and recompute failed\n");
            return -1;
    }
    else if(rv == 0) {
	    fprintf(stderr, "the nodes were the same\n");
	    return -1;
    }
    else if(rv <= 0) {
	    fprintf(stderr, "the nodes hash were different and recomputed\n");
            return -1;
    }

    char path_diff [] = "./example/metadatas/dfs_num_nodes.txt";
    if(mt_dfs_write(mt, path_diff) == -1) {
	    fprintf(stderr, "the mt_dfs_write has failed\n");
	    return -1;
    }
 
    free_merkle(mt);
    free(mt);
    //fprintf(stderr, "\n");
    return 0;
}
int main(void) {

    if(test_case1() == -1) {
	    fprintf(stderr, "test case 1 failed\n"); 
	    return -1;
    }
    
    if(test_case2() == -1) {
	    fprintf(stderr, "test case 2 failed\n");
            return -1;
    }
    
    if(test_case3() == -1) {
	    fprintf(stderr, "test case 3 failed\n");
            return -1;
    }

    printf("0");
    return 0;
}
