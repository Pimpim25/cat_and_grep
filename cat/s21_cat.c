#include <stdio.h>
#include <string.h>

#include "../common/common.h"

int parc_arguments(int argc, char **argv, int long_flags_count,
                   int *files_count, int error_flag, char flags[100],
                   int index_flags, char current_flags[6],
                   char *current_files[100], int type);
void use_flags(char *current_flags, char **current_files, int files_count);
int sort_long_flags(char *current_flags, char *argv);
void use_s_flag(char buff[4096], int *str_s);
void check_e_flag(char buff[4096], int e_flag, char extended[4096]);
void use_tv_flags(char extended[4096], int t_flag, int v_flag);
void turn_on_flags(int *b_flag, int *e_flag, int *n_flag, int *s_flag,
                   int *t_flag, int *v_flag, char *current_flags);

int main(int argc, char **argv) {
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
  _setmode(_fileno(stdout), _O_BINARY);
#endif

  char flags[100] = {0};
  char current_flags[6] = {0};
  char *current_files[100] = {0};
  char static_flags[] = {'e', 'b', 'n', 's', 't', 'v', '\0'};
  int index_flags = 0;
  int files_count = 0;
  int error_flag = 1;
  int type = 0;
  int long_flags_count = 0;

  error_flag =
      parc_arguments(argc, argv, long_flags_count, &files_count, error_flag,
                     flags, index_flags, current_flags, current_files, type);
  if (error_flag != 0) {
    error_flag = sort_short_flags(flags, current_flags, static_flags, type);
  }
  if (error_flag != 0) {
    use_flags(current_flags, current_files, files_count);
  }
  return 0;
}

int parc_arguments(int argc, char **argv, int long_flags_count,
                   int *files_count, int error_flag, char flags[100],
                   int index_flags, char current_flags[6],
                   char *current_files[100], int type) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] > 96 && argv[i][1] < 125) {
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        flags[index_flags++] = argv[i][j];
      }
    } else if (argv[i][0] == '-' && (argv[i][1] == '-' || argv[i][1] == 'T' ||
                                     argv[i][1] == 'E' || argv[i][1] == 'v')) {
      error_flag = sort_long_flags(current_flags, argv[i]);
      long_flags_count++;
    } else {
      if (check_file_exist(argv[i], current_flags, type) != 0) {
        current_files[(*files_count)++] = argv[i];
      } else {
        error_flag = 0;
      }
    }
  }
  return error_flag;
}

int sort_long_flags(char *current_flags, char *argv) {
  int b = 0;
  int error_flag = 1;
  char static_flags[] = {'b', 'e', 'n', 's', 't', 'v'};
  const char *long_flags[] = {"--number-nonblank", "-E", "--number",
                              "--squeeze-blank",   "-T", "-v"};
  for (int i = 0; i < 6; i++) {
    if (strcmp(argv, long_flags[i]) == 0) {
      current_flags[i] = static_flags[i];
      b++;
    }
  }
  if (b == 0) {
    error_flag = 0;
    printf("Flag is not found.\n");
  }
  return error_flag;
}

void use_flags(char *current_flags, char **current_files, int files_count) {
  int str_s = 0;
  int str_n = 1;
  int b_flag = 0;
  int e_flag = 0;
  int n_flag = 0;
  int s_flag = 0;
  int t_flag = 0;
  int v_flag = 0;
  char extended[4096] = {0};
  char end = '\n';
  turn_on_flags(&b_flag, &e_flag, &n_flag, &s_flag, &t_flag, &v_flag,
                current_flags);
  char buff[4096];
  for (int j = 0; j < files_count; j++) {
    FILE *f = fopen(current_files[j], "r");
    if (f != NULL) {
      while (fgets(buff, sizeof(buff), f) != NULL) {
        if (s_flag) {
          use_s_flag(buff, &str_s);
          if (buff[0] == '\0') {
            continue;
          }
        }
        int is_non_empty = (buff[0] != '\n');
        check_e_flag(buff, e_flag, extended);
        if (b_flag && end == '\n') {
          if (is_non_empty) {
            printf("%6d\t", str_n++);
          }
        } else if (n_flag && !b_flag && end == '\n') {
          printf("%6d\t", str_n++);
        }
        use_tv_flags(extended, t_flag, v_flag);
        size_t len = strlen(extended);
        end = extended[len - 1];
      }
      fclose(f);
    }
  }
}

void use_s_flag(char buff[4096], int *str_s) {
  if (buff[0] == '\n') {
    (*str_s)++;
    if (*str_s > 1) {
      buff[0] = '\0';
      (*str_s)--;
    }
  } else {
    *str_s = 0;
  }
}

void check_e_flag(char buff[4096], int e_flag, char extended[4096]) {
  size_t len = strlen(buff);
  if (e_flag) {
    if (len > 0 && buff[len - 1] == '\n') {
      buff[len - 1] = '\0';
      snprintf(extended, 4096, "%s$\n", buff);
    } else {
      snprintf(extended, 4096, "%s", buff);
    }
  } else {
    strncpy(extended, buff, 4095);
  }
}

void use_tv_flags(char extended[4096], int t_flag, int v_flag) {
  for (size_t k = 0; k < strlen(extended); k++) {
    unsigned char c = extended[k];
    if (t_flag && c == '\t') {
      printf("^I");
      continue;
    }
    if (v_flag) {
      if (c < 32 && c != '\n' && c != '\t') {
        printf("^%c", c + 64);
        continue;
      } else if (c == 127) {
        printf("^?");
        continue;
      }
    }
    putchar(c);
  }
}

void turn_on_flags(int *b_flag, int *e_flag, int *n_flag, int *s_flag,
                   int *t_flag, int *v_flag, char *current_flags) {
  for (int i = 0; i < 6; i++) {
    if (current_flags[i] == 'b') *b_flag = 1;
    if (current_flags[i] == 'n') *n_flag = 1;
    if (current_flags[i] == 's') *s_flag = 1;
    if (current_flags[i] == 'e') *e_flag = 1;
    if (current_flags[i] == 't') *t_flag = 1;
    if (current_flags[i] == 'v') *v_flag = 1;
  }
}
