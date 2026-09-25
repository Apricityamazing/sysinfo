#include "version.h"
#include "findvalues.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *get_version(void) {
  KeyValuePair *attributes[] = {
      create_keyvaluepair("Linux version ", SEARCH_BEFORESTRING, " "),
  };

  uint8_t num_attributes = sizeof(attributes) / sizeof(KeyValuePair *);
  char *version;
  find_values("/proc/version", num_attributes, attributes);
  if (attributes[0]->value != NULL) {
    version = malloc(strlen(attributes[0]->value) + 1);
    strcpy(version, attributes[0]->value);
  } else {
    version = NULL;
  }
  destroy_keyvaluepair(attributes[0]);
  return version;
}
