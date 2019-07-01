#include <string.h>
#include <stdio.h>
#include "p_json.h"


// e.g. json={"id": "myid"}, key="id":, buff=myid
void json_string(const char *json, const char *key, char *buff) {
  char *k = strstr(json, key);
  char *start = strchr(k+strlen(key), '\"')+1;
  char *end = strchr(start, '\"');
  memcpy(buff, start, end-start);
  buff[end-start] = '\0';
}
