#include <stdcountof.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_value(char line[256], char *output, size_t output_size) {
  char line_copy[256];
  strcpy(line_copy, line);

  char *ptr = line_copy;
  // Removes leading model name :
  strsep(&ptr, ":");
  char *value = strsep(&ptr, "\n");
  if (value != NULL) {
    // Removes leading whitespace
    while (*value == ' ' || *value == '\t') {
      value++;
    }
  }
  strncpy(output, value, output_size - 1);
  output[output_size - 1] = '\0'; // Makes sure its null terminated
}

int main(void) {
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  char line[256];
  char model[256];
  if (cpuinfo == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, sizeof(line), cpuinfo) != NULL) {
    if (strstr(line, "model name") != NULL) {
      // Changes value of model to the value
      get_value(line, model, sizeof(model));
      break;
    }
  }

  fclose(cpuinfo);
  printf("Model: %s\n", model);
  return EXIT_SUCCESS;
}
