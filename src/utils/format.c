#include "utils.h"

void print_hostent(struct hostent *s) {
  int i;
  struct in_addr **addr_list;
  fprintf(stderr, "[overload] name : %s\n", s->h_name);
  for(i=0;s->h_aliases && s->h_aliases[i];i++) {
    fprintf(stderr, "[overload] alias : %s\n", s->h_aliases[i]);
  }
  fprintf(stderr, "[overload] length : %d\n", s->h_length);
  addr_list = (struct in_addr**)s->h_addr_list;
  for(i=0;addr_list[i];i++) {
    fprintf(stderr, "[overload] addr : %s\n", inet_ntoa(*addr_list[i]));
  }
}
