#include "destor.h"
#include "jcr.h"
#include "backup.h"

static pthread_t hash_t;
static int64_t chunk_num;
/*
void build_mt(fingerprint * leaves) {
	int64_t hash_num = chunk_num;
	
	while((hash_num /= 2) > 0) {
		for(int i = 0; i < hash_num; i++) {
			struct chunk* c = new_chunk(0); 
			TIMER_DECLARE(1);
			TIMER_BEGIN(1);
			SHA_CTX ctx;
			SHA_Init(&ctx);
			SHA_Update(&ctx, leaves[2*i], 20);
			SHA_Update(&ctx, leaves[2*i + 1], 20);
			SHA_Final(c->fp, &ctx); 
			TIMER_END(1, jcr.hash_time);
			sync_queue_push(hash_queue, c);
		}
	}
	return;
}
*/

static void* sha1_thread(void* arg) {
	char code[41];
	int curr_size = 128;
	fingerprint * leaves = malloc(sizeof(fingerprint) * curr_size);
	
	while (1) {
		struct chunk* c = sync_queue_pop(chunk_queue);

		if (c == NULL) {
			sync_queue_term(hash_queue);
			break;
		}
		
		if (CHECK_CHUNK(c, CHUNK_FILE_START) || CHECK_CHUNK(c, CHUNK_FILE_END)) {
			sync_queue_push(hash_queue, c);
			continue;
		}

		TIMER_DECLARE(1);
		TIMER_BEGIN(1);
		SHA_CTX ctx;
		SHA_Init(&ctx);
		SHA_Update(&ctx, c->data, c->size);
		SHA_Final(c->fp, &ctx);
		TIMER_END(1, jcr.hash_time);

		hash2code(c->fp, code);
		code[40] = 0;
		VERBOSE("Hash phase: %ldth chunk identified by %s", chunk_num++, code);
		//printf("this is chunk_num %ld\n", chunk_num);
		sync_queue_push(hash_queue, c);
		memcpy(leaves[chunk_num - 1], c->fp, sizeof(fingerprint));
		
	}
	
	return NULL;
}

void start_hash_phase() {
	hash_queue = sync_queue_new(128);
	pthread_create(&hash_t, NULL, sha1_thread, NULL);
}

void stop_hash_phase() {
	pthread_join(hash_t, NULL);
	NOTICE("hash phase stops successfully!");
}
