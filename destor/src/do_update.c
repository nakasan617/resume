#include "destor.h"
#include "jcr.h"
#include "utils/sync_queue.h"
#include "index/index.h"
#include "backup.h"
#include "storage/containerstore.h"

void do_update(char *path) {
	struct stat s;
	if(stat(path, &s) != 0) {
		fprintf(stderr, "update file does not exist!\n");
		exit(1);
	}

	if(ptree == NULL) {
		fprintf(stderr, "POS tree has not been created yet!\n");
		exit(1);
	}

	FILE * fd = fopen(path, "r");

	fclose(fd);
	return;
}
