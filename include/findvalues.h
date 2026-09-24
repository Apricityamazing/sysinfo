#ifndef FINDVALUE_H
#define FINDVALUE_H
typedef enum {
  SEARCH_FIRST, // Finds the first instance of the key and extracts the value
  SEARCH_COUNT, // Counts up from 0 for the number of instances the key is found
} SearchMode;

typedef struct {
  const char *key;
  char *value;
  SearchMode mode;
} KeyValuePair;

KeyValuePair *create_keyvaluepair(const char *key, SearchMode mode);
void destroy_keyvaluepair(KeyValuePair *keyvaluepair);
void find_values(char *filepath, int num_pairs, KeyValuePair *keys[]);
#endif
