/* I have referred to https://stackoverflow.com/questions/1271064/how-do-i-loop-through-all-files-in-a-folder-using-c  to make this code below */
#include "main.h"
int verbosity = POS_VERBOSE;

/*
void pos_log(int level, const char *fmt, ...) {
    va_list ap;
    char msg[1024];
    if((level & 0xff) < verbosity) 
        return;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    fprintf(stdout, "%s\n", msg);
}
*/

int main(int argc, char **argv) {
    int file_count = 0;
    FILE *fp = fopen("times.txt", "w");
    fclose(fp);

    struct dirent *dp; 
    DIR *dfd;
    char *dir;
    dir = argv[1];

    if(argc == 1) {
        fprintf(stderr, "Usage: %s dirname\n", argv[0]);
        return 0;
    }

    if((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "Cannot open %s\n", dir);
        return 0;
    }
    node ** roots = malloc(sizeof(node *) * 32);
    hm_root *hr = NULL;

    hashmap *hm = hashmap_create(HASHMAP_SIZE, node_destroy);

    char filename_qfd[128];
    int root_cnt = 0;

    while((dp = readdir(dfd)) != NULL) {
        struct stat stbuf;
        
        if(dir[strlen(dir) - 1] == '/') {
            sprintf(filename_qfd, "%s%s", dir, dp->d_name);
        } else {
            sprintf(filename_qfd, "%s/%s", dir, dp->d_name);
        }

        if(stat(filename_qfd, &stbuf) == -1) {
            printf("Unable to stat file: %s\n", filename_qfd);
            continue;
        }
        if((stbuf.st_mode & S_IFMT) == S_IFDIR)
            continue;
        else {
            //printf("filename: %s: %d\n", filename_qfd, root_cnt);
            hr = iteration(filename_qfd, hm, file_count++);
            if(!hr) continue;
            if(!root_cnt ||  hr->root != roots[root_cnt - 1]) {
                roots[root_cnt++] = hr->root;
            }
            if(root_cnt % 32 == 0)
                roots = realloc(roots, sizeof(node *) *(root_cnt + 32));
            if(!roots) {
                printf("not enough memory\n");
                exit(1);
            }
            free(hr);
            hr = NULL; 
        }
    }
    closedir(dfd);
    roots = realloc(roots, sizeof(node *)*root_cnt);
    
    //printf("\nwrite_phase\n");
    hashmap *written = hashmap_create(HASHMAP_SIZE, node_destroy);
    
    //printf("\npos_write phase\n");
    for(int i = 0; i < root_cnt; ++i) {
        POS_write(written, roots[i], i);     
    }

    /* freeing everything */
    //printf("\ndestroy phase\n");
    free(roots); 
    //printf("destroy written\n");
    assert(!hashmap_destroy(written, HMONLY)); 
    //printf("destroy hm\n");
    assert(!hashmap_destroy(hm, WHOLE));
    if(hr) free(hr);

    //printf("main done\n");
    return 0;
}

hm_root *iteration(char path[], hashmap *hm, int file_count) {
    struct timespec start, end;
    int leaves_count = 0;
    int node_difference = 0;
    int total_nodes = 0;
    char **parsed = parse(path, &leaves_count);
    key_value **leaves = parse_kvs(parsed, leaves_count);

    assert(!clock_gettime(CLOCK_MONOTONIC, &start));
    hm_root *hr = create_POS(leaves, leaves_count, hm, &node_difference, &total_nodes);
    assert(!clock_gettime(CLOCK_MONOTONIC, &end));
    uint64_t diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

    for(int i = 0; i < leaves_count; i++) {
        free(parsed[i]);
    }
    free(parsed);

    for(int i = 0; i < leaves_count; i++) {
        free(leaves[i]->key);
        free(leaves[i]->value);
        free(leaves[i]);
    }
    free(leaves);

    FILE *fp = fopen("times.txt", "a+");
    fprintf(fp, "%s:%d:%d:%d:%llu,",path, leaves_count, total_nodes, node_difference, (long long unsigned int) diff);
    fclose(fp);
    //fprintf(stderr, "%s:%llu,",path, (long long unsigned int) diff);
    return hr;
}


