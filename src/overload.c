#include<stdio.h>
#include<string.h>
#include<netdb.h>

#define __USE_GNU
#include <dlfcn.h>

static void* (*real_gethostbyname)(const char*)=NULL;
static int (*real_getaddrinfo)(const char* node, const char *service, const struct addrinfo *hints, struct addrinfo **res)=NULL;

static void __gethostbyname_init(void) {
  real_gethostbyname = dlsym(RTLD_NEXT, "gethostbyname");
  if(NULL == real_gethostbyname) {
    fprintf(stderr, "[overload:gethostbyname:dlsym] Error in dlsym : %s\n", dlerror());
    return;
  }
}

static void __getaddrinfo_init(void) {
  real_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
  if(NULL == real_getaddrinfo) {
    fprintf(stderr, "[overload:getaddrinfo:dlsym] Error in dlsym : %s\n", dlerror());
    return;
  }
}


struct hostent *gethostbyname(const char *name) {
  if(NULL == real_gethostbyname) {
    __gethostbyname_init();
  }

  void * p = NULL;
  /*  struct hostent *response = (struct hostent*)malloc(sizeof struct hostent);
  response->h_name = "www.google.com";
  response->h_aliases = { "google", NULL };
  response->h_addrtype = AF_INET;
  response->h_length = strlen("1.1.1.1");
  response->h_addr = {'1.1.1.1', NULL};
  if(strncmp(name, "www.google.com", strlen("www.google.com") == 0)) {
      return response;
  }
  free(response);
  */

  fprintf(stderr, "[overload:gethostbyname] Looking up %s\n", name);
  p = real_gethostbyname(name);
  return p;
}


int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){
  if(NULL == real_getaddrinfo)
    __getaddrinfo_init();
  fprintf(stderr, "[overload:getaddrinfo] looking up %s\n", node);
  return real_getaddrinfo(node, service, hints, res);
}
