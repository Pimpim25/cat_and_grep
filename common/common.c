#include "common.h"

#include <stdio.h>
#include <string.h>

int check_file_exist(char *file, char *current_flags, int type) {
  int error_flag = 1;
  FILE *fp = fopen(file, "r");

  if (fp != NULL) {
    fclose(fp);
  } else {
    error_flag = 0;
    if (current_flags[8] != 115 && !type) {
      printf("File is not found or does not exist.\n");
    }
  }
  return error_flag;
}

int sort_short_flags(char *user_flags, char *current_flags, char *static_flags,
                     int type) {
  int b;
  int error_flag = 1;
  size_t len = strlen(static_flags);
  for (size_t i = 0; i < strlen(user_flags); i++) {
    b = 0;
    for (size_t j = 0; j < len; j++) {
      if (user_flags[i] == static_flags[j]) {
        current_flags[j] = static_flags[j];
        b++;
      }
      if (type == 0 &&
          ((user_flags[i] == 101) || (user_flags[i] == static_flags[4]))) {
        current_flags[5] = static_flags[5];
        b++;
      }
    }
    if (b == 0) {
      error_flag = 0;
      printf("Flag is not found.\n");
    }
  }
  return error_flag;
}