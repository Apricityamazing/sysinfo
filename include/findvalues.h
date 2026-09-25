#ifndef FINDVALUE_H
#define FINDVALUE_H
typedef enum {
  SEARCH_FIRST, // Finds the first instance of the key and extracts the value
  SEARCH_COUNT, // Counts up from 0 for the number of instances the key is found
  SEARCH_BEFORESTRING, // Get the value after the key, and before the first
                       // instance of string
} SearchMode;

typedef struct {
  const char *key;
  char *value;
  const char
      *string; // Only needed for SEARCH_BEFORESTRING, set as NULL otherwise
  SearchMode mode;
} KeyValuePair;

KeyValuePair *create_keyvaluepair(const char *key, SearchMode mode,
                                  const char *string);
void destroy_keyvaluepair(KeyValuePair *keyvaluepair);
void find_values(char *filepath, int num_pairs, KeyValuePair *keys[]);
#endif
