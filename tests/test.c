#include<stdio.h>
#include<netdb.h>

int main(void) { 
  struct hostent *p = NULL;
  const char *name = "www.google.com";
  fprintf(stderr, "[test.c] Looking up %s\n", name);
  p = gethostbyname(name);
  return 0;
}
