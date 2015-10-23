#include<stdio.h>
#include<netdb.h>

#include "overload.h"
#include "utils.h"

int main(void) {
	Buffer *b;
	struct hostent *p = NULL;
	struct hostent *q = NULL;
	const char *name = "www.groupon.com";

	fprintf(stderr, "[test.c] Looking up %s\n", name);
  p = gethostbyname(name);

	fprintf(stdout, "Before serialization\n");
	print_hostent(p);
	b = serialize_hostent(p);

	q = deserialize_hostent(b);
	fprintf(stdout, "After serialiation\n");
	print_hostent(q);

	free(b);
	free(q);


  return 0;
}
