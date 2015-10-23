#include "utils.h"
#include<string.h>
#include<stdlib.h>

int main(void) {
	MDB_env *env;
	Buffer *b;
	void *v;
	size_t v_sz;

	char *path = "./tmp";
	struct hostent *p = NULL;
	struct hostent *q = NULL;
	const char *name = "www.google.com";

	if(slb_db_open(&env, path) < 0) {
		exit(1);
	}

	fprintf(stderr, "[test.c] Looking up %s\n", name);
  	p = gethostbyname(name);

	fprintf(stdout, "Before serialization\n");
	print_hostent(p);
	b = serialize_hostent(p);

	v = serialize_buffer(b);
	v_sz = b->size + sizeof(int);
	if(slb_db_write(&env, (char*)name, v, v_sz) < 0) {
		fprintf(stderr, "error");
		exit(1);
	}

	free(v);
	// free(b);
	// b = NULL;
	void *data = malloc(MAXDATASIZE);
	bzero(data, MAXDATASIZE);
	if(slb_db_read(&env, (char*)name, &data)<0){
		exit(1);
	}

	b = deserialize_void(data);
	q = deserialize_hostent(b);
	fprintf(stdout, "After serialiation\n");
	print_hostent(q);

	free(b);
	free(q);


	slb_db_close(&env);
}
