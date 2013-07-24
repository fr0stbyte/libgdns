#include<stdio.h>
#include<ares.h> 

#define __USE_GNU
#include <dlfcn.h>

static void* (*real_ares_gethostbyname)(ares_channel channel, const char *name, int family, ares_host_callback callback, void *arg)=NULL;

void __ares_gethostbyname_init() {
  real_ares_gethostbyname = dlsym(RTLD_NEXT, "ares_gethostbyname");
  if(NULL == real_ares_gethostbyname) {
    fprintf(stderr, "[ares_overload] Error in dlsym\n");
    return ;
  }
}


void ares_gethostbyname(ares_channel channel, const char *name, int family, ares_host_callback callback, void *arg) {
  if(NULL == real_ares_gethostbyname)
    __ares_gethostbyname_init();

  fprintf(stderr, "[ares_overload] looking up %s\n", name);
  
  real_ares_gethostbyname(channel, name, family, callback, arg);
}
