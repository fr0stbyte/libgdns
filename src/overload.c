#include "overload.h"
#include "utils.h"

static void* (*real_gethostbyname)(const char*)=NULL;
static int (*real_getaddrinfo)(const char* node, const char *service, const struct addrinfo *hints, struct addrinfo **res)=NULL;

static MDB_env *env;

static void __gethostbyname_init(void) {
  real_gethostbyname = dlsym(RTLD_NEXT, "gethostbyname");
  if(NULL == real_gethostbyname) {
    fprintf(stderr, "[overload:gethostbyname:dlsym] Error in dlsym : %s\n", dlerror());
    return;
  }

  slb_db_open(&env, "/var/tmp/slb");
}

static void __getaddrinfo_init(void) {
  real_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
  if(NULL == real_getaddrinfo) {
    fprintf(stderr, "[overload:getaddrinfo:dlsym] Error in dlsym : %s\n", dlerror());
    return;
  }
}


struct hostent *gethostbyname(const char *name) {
  if(NULL == real_gethostbyname)
    __gethostbyname_init();

  struct hostent *p;
  Buffer *b = NULL;
  void *value = malloc(MAXDATASIZE);
  bzero(value, MAXDATASIZE);
  fprintf(stderr, "[overload:gethostbyname] Looking up %s\n", name);

  switch(slb_db_read(&env, (char*)name, &value)) {
    case -1:
      fprintf(stderr, "error\n");
      break;
    case 0:
      b = deserialize_void(value);
      p = deserialize_hostent(b);
      break;
    case 1:
      p = real_gethostbyname(name);
      b = serialize_hostent(p);
      void* data = serialize_buffer(b);
      size_t data_sz = b->size + sizeof(int);
      slb_db_write(&env, (char*)name, data, data_sz);
      free(data);
      break;
    default:
      // should not get here
      fprintf(stderr, "[slb_db_read] return code not within known range\n");
  }
  print_hostent(p);
  if(b) free(b);
  free(value);
  return p;
}


int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){
  if(NULL == real_getaddrinfo)
    __getaddrinfo_init();
  fprintf(stderr, "[overload:getaddrinfo] looking up %s\n", node);
  return real_getaddrinfo(node, service, hints, res);
}
