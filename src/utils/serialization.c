#include "utils.h"


Buffer * new_buffer(void) {
	Buffer *b = malloc(sizeof(Buffer));

	b->data = malloc(INITIAL_HOSTENT_SIZE);
	b->size = INITIAL_HOSTENT_SIZE;
	b->next = 0;

	bzero(b->data, INITIAL_HOSTENT_SIZE);
	return b;
}

void reserve_space(Buffer *buf, size_t bytes) {
	if(buf->next + bytes > buf->size) {
		buf->data = realloc(buf->data, buf->size*2);
		buf->size = buf->size*2;
	}
}

void serialize_string(Buffer *buf, char* str) {
	size_t sz = strlen(str);
	reserve_space(buf, sz);

	memcpy((char *)buf->data + buf->next, str, sz);
	buf->next += sz;
}

void serialize_int(Buffer *buf, int v) {
	size_t sz = sizeof(int);
	reserve_space(buf, sz);

	memcpy((char*)buf->data + buf->next, &v, sz);
	buf->next += sz;
}

int deserialize_int(Buffer *b) {
	int x;
	size_t sz = sizeof(int);
	memcpy(&x, (char*)b->data + b->next, sz);
	b->next += sz;

	return x;
}

char* deserialize_string(Buffer *b, size_t sz){
	if(sz == 0)
		return NULL;
	char* str = (char*)malloc(sz*sizeof(char));
	memcpy(str, (char*)b->data + b->next, sz);
	b->next += sz;

	return str;
}

Buffer* serialize_hostent(struct hostent* s){
	int i;
	struct in_addr **addr_list;
	Buffer *b = new_buffer();

	// h_name
	if(strlen(s->h_name)) {
		serialize_int(b, strlen(s->h_name));
		serialize_string(b, s->h_name);
	} else {
		serialize_int(b, 0);
	}

	if(s->h_aliases) {
		i=0;
		while(s->h_aliases[i]) { ++i; }
		serialize_int(b,i);
		for(i=0;s->h_aliases[i];++i){
			serialize_int(b, strlen(s->h_aliases[i]));
			serialize_string(b, s->h_aliases[i]);
		}
	} else {
		serialize_int(b, 0);
	}

	serialize_int(b, s->h_addrtype);

	serialize_int(b, s->h_length);

	if(s->h_addr_list) {
		i = 0;
		while(s->h_addr_list[i]) {i++;}
		serialize_int(b, i);
		for(i=0;s->h_addr_list[i];i++) {
			serialize_int(b, strlen(s->h_addr_list[i]));
			serialize_string(b, s->h_addr_list[i]);
		}
	} else {
		serialize_int(b, 0);
	}
	if(s->h_addr) {
		serialize_int(b, s->h_length);
		serialize_string(b, s->h_addr);
	} else {
		serialize_int(b, 0);
	}
	b->next = 0;
	return b;
}

struct hostent* deserialize_hostent(Buffer *b) {
	int x,i;
	size_t sz;

	struct hostent *p = NULL;
	p = (struct hostent*)malloc(sizeof(struct hostent));
	bzero(p, sizeof(struct hostent));
	// check for h_name
	x = deserialize_int(b);
	if(x>0){
		p->h_name = deserialize_string(b, x);
	} else {
		p->h_name = '\0';
	}
	//check for h_aliases
	x = deserialize_int(b);
	if(x>0){
		p->h_aliases = malloc(sizeof(char*)*(x+1));
		for(i=0;i<x;i++){
			sz = deserialize_int(b);
			p->h_aliases[i] = deserialize_string(b, sz);
		}
		p->h_aliases[i] = NULL;
	} else {
		p->h_aliases = NULL;
	}

	p->h_addrtype = deserialize_int(b);
	p->h_length = deserialize_int(b);

	// check for h_addr_list
	x = deserialize_int(b);
	if(x>0){
		// for some ODD reason p->h_addr_list[x] points to p->h_name
		p->h_addr_list = malloc(sizeof(char*)*(x+1));
		for(i=0;i<x;i++){
			sz = deserialize_int(b);
			p->h_addr_list[i] = deserialize_string(b, sz);
		}
		p->h_addr_list[i] = NULL;
	} else {
		p->h_addr_list = NULL;
	}

	sz = deserialize_int(b);
	if(sz>0){
		p->h_addr = deserialize_string(b, sz);
	}
	return p;
}

void* serialize_buffer(Buffer* b){
	if(!b || !b->data || b->size == 0) {
		return NULL;
	}
	size_t int_sz = sizeof(int);
	size_t sz = b->size + int_sz;

	void* data = malloc(sz);
	bzero(data, sz);
	memcpy((char*)data, &(b->size), int_sz);
	memcpy((char*)data + int_sz, (char*)b->data, b->size);
	return data;
}

Buffer* deserialize_void(void* data) {
	int x;
	Buffer* b = new_buffer();
	memcpy(&x, (char*)data, sizeof(int));
	reserve_space(b, x);
	memcpy((char*)b->data, (char*)data+sizeof(int), x);
	b->size = x;
	b->next = 0;

	return b;
}
