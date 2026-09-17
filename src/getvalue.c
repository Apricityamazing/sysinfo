#include "getvalue.h"
#include <stdlib.h>
#include <string.h>
char *get_value(char *buffer) {
  char *ptr = buffer;
  // Removes leading 'key :'
  strsep(&ptr, ":");
  char *value = strsep(&ptr, "\n");
  if (value != NULL) {
    // Removes leading whitespace
    while (*value == ' ' || *value == '\t') {
      value++;
    }
  }
  char *output = malloc(strlen(value) + 3);
  strcpy(output, value);
  return output;
}
