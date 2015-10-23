#ifndef __LIBGDNS_UTILS_H__
#define __LIBGDNS_UTILS_H__

#define MAXKEYSIZE 511
#define MAXDATASIZE 0xffffUL
#define INITIAL_HOSTENT_SIZE 128

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<lmdb.h>
#include<netdb.h>
#include<arpa/inet.h>


// LMDB manipulation
int slb_db_open(MDB_env **env, char *path);
int slb_db_read(MDB_env **env, char *key, void **v);
int slb_db_write(MDB_env **env, char *key, void *v, size_t v_sz);
int slb_db_del(MDB_env **env, char *key);
void slb_db_close(MDB_env **env);


// serialization
typedef struct {
	void *data;
	int next;
	size_t size;
}Buffer;

Buffer* new_buffer();
void reserve_space(Buffer *buf, size_t bytes);
void serialize_string(Buffer *buf, char* str);
void serialize_int(Buffer *buf, int v);

int deserialize_int(Buffer *b);
char* deserialize_string(Buffer *b, size_t sz);

Buffer* serialize_hostent(struct hostent *p);
struct hostent* deserialize_hostent(Buffer *b);

void* serialize_buffer(Buffer*);
Buffer* deserialize_void(void*);


// pretty printing
void print_hostent(struct hostent *s);
#endif
