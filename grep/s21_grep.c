#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

#define MAX_COUNT 100
#define MAX_LENGTH 1024

typedef struct {
  int start;
  int end;
} Match;

int parc_arguments(int argc, char **argv, char *current_flags,
                   char files[][MAX_LENGTH], int *files_count,
                   char pattern[][MAX_LENGTH], int *pattern_count,
                   int *counte_flags, int *index_flags, int *type,
                   char static_flags[], char flags[MAX_COUNT][16]);
void add_flags(char *flag_str, char *dest_flags, int *index_flags);
void add_pattern(char pattern[][MAX_LENGTH], int *pattern_count,
                 const char *src);
int creating_pattern(char *argv, char pattern[][MAX_LENGTH],
                     int *pattern_count);
void use_flags(char *current_flags, char files[][MAX_LENGTH], int files_count,
               regex_t *regex, char pattern[][MAX_LENGTH], int pattern_count);
void use_c_flag(int files_count, regex_t *regex, char files[][MAX_LENGTH],
                char *buff, int pattern_count, int v_flag, int h_flag);
void use_l_flag(int files_count, regex_t *regex, char files[][MAX_LENGTH],
                char *buff, int pattern_count, int v_flag);
void use_eivnho_flag(int files_count, regex_t *regex, char files[][MAX_LENGTH],
                     char *buff, int pattern_count, int v_flag, int n_flag,
                     int h_flag, int o_flag);
int create_regular(int pattern_count, regex_t *regex, int E_flag, int i_flag,
                   char pattern[][MAX_LENGTH]);
void use_o_flag(regmatch_t match[1], char *buff, regex_t *regex,
                int files_count, int h_flag, int j, char files[][MAX_LENGTH],
                int *matched, int n_flag, int line_number, int pattern_count);

int main(int argc, char **argv) {
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
  _setmode(_fileno(stdout), _O_BINARY);
#endif
  regex_t regex[100];
  int counte_flags = 0;
  int index_flags = 0;
  int files_count = 0;
  int pattern_count = 0;
  int error_flag = 1;
  int type = 1;

  char flags[MAX_COUNT][16] = {{0}};
  char current_flags[11] = {0};
  char static_flags[] = {'e', 'i', 'v', 'c', 'l', 'n',
                         'E', 'h', 's', 'f', 'o', '\n'};
  char pattern[MAX_COUNT][MAX_LENGTH] = {{0}};
  char files[MAX_COUNT][MAX_LENGTH] = {{0}};

  error_flag = parc_arguments(argc, argv, current_flags, files, &files_count,
                              pattern, &pattern_count, &counte_flags,
                              &index_flags, &type, static_flags, flags);
  for (int i = 0; i < MAX_COUNT; i++) {
    if (error_flag) {
      error_flag =
          sort_short_flags(flags[i], current_flags, static_flags, type);
    }
  }
  if (error_flag != 0) {
    use_flags(current_flags, files, files_count, regex, pattern, pattern_count);
  }
  return 0;
}

int parc_arguments(int argc, char **argv, char *current_flags,
                   char files[][MAX_LENGTH], int *files_count,
                   char pattern[][MAX_LENGTH], int *pattern_count,
                   int *counte_flags, int *index_flags, int *type,
                   char static_flags[], char flags[MAX_COUNT][16]) {
  int error_flag = 1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      add_flags(argv[i], flags[*counte_flags], index_flags);
      if (i == 1 && strchr(flags[0], 'e') == NULL &&
          strchr(flags[0], 'f') == NULL) {
        flags[*counte_flags][(*index_flags)++] = 'e';
        flags[*counte_flags][*index_flags] = '\0';
      }
    } else {
      *index_flags = 0;
      (*counte_flags)++;
      if (strchr(flags[(*counte_flags) - 1], 'e') != NULL &&
          strchr(flags[(*counte_flags) - 1], 'f') == NULL) {
        add_pattern(pattern, pattern_count, argv[i]);
      } else if ((*counte_flags) == 1 &&
                 strchr(flags[(*counte_flags) - 1], 'f') == NULL) {
        add_pattern(pattern, pattern_count, argv[i]);
        flags[*counte_flags][0] = 'e';
        flags[(*counte_flags)++][1] = '\0';
      } else {
        if (strchr(flags[(*counte_flags) - 1], 'f') != NULL) {
          error_flag = creating_pattern(argv[i], pattern, pattern_count);
        } else {
          if (error_flag) {
            for (int k = 0; k < (*counte_flags); k++) {
              error_flag = sort_short_flags(flags[k], current_flags,
                                            static_flags, *type);
            }
          }
          if (error_flag != 0) {
            if (check_file_exist(argv[i], current_flags, *type)) {
              strncpy(files[*files_count], argv[i], MAX_LENGTH - 1);
              files[(*files_count)++][MAX_LENGTH - 1] = '\0';
            } else if (current_flags[8] != 115) {
              error_flag = 0;
            }
          }
        }
      }
    }
  }
  return error_flag;
}

void add_flags(char *flag_str, char *dest_flags, int *index_flags) {
  for (size_t j = 1; j < strlen(flag_str); j++) {
    dest_flags[(*index_flags)++] = flag_str[j];
  }
  dest_flags[*index_flags] = '\0';
}

void add_pattern(char pattern[][MAX_LENGTH], int *pattern_count,
                 const char *src) {
  strncpy(pattern[*pattern_count], src, MAX_LENGTH - 1);
  pattern[(*pattern_count)++][MAX_LENGTH - 1] = '\0';
}

int creating_pattern(char *argv, char pattern[][MAX_LENGTH],
                     int *pattern_count) {
  char buff[4096];
  int error_flag = 1;
  FILE *f = fopen(argv, "r");
  if (f != NULL) {
    while (fgets(buff, MAX_LENGTH, f)) {
      char *ptr;
      if ((ptr = strchr(buff, '\r')) != NULL) *ptr = '\0';
      if ((ptr = strchr(buff, '\n')) != NULL) *ptr = '\0';
      strncpy(pattern[(*pattern_count)], buff, MAX_LENGTH - 1);
      (*pattern_count)++;
    }
  } else {
    error_flag = 0;
  }
  return error_flag;
}

void use_flags(char *current_flags, char files[][MAX_LENGTH], int files_count,
               regex_t *regex, char pattern[][MAX_LENGTH], int pattern_count) {
  char buff[4096];
  int error_flag = 0;
  int i_flag = 0;
  int v_flag = 0;
  int c_flag = 0;
  int l_flag = 0;
  int n_flag = 0;
  int E_flag = 0;
  int h_flag = 0;
  int o_flag = 0;

  for (int i = 0; i < 11; i++) {
    if (current_flags[i] == 'i') i_flag = 1;
    if (current_flags[i] == 'v') v_flag = 1;
    if (current_flags[i] == 'c') c_flag = 1;
    if (current_flags[i] == 'l') l_flag = 1;
    if (current_flags[i] == 'n') n_flag = 1;
    if (current_flags[i] == 'E') E_flag = 1;
    if (current_flags[i] == 'h') h_flag = 1;
    if (current_flags[i] == 'o') o_flag = 1;
  }

  error_flag = create_regular(pattern_count, regex, E_flag, i_flag, pattern);

  if (!error_flag) {
    if (l_flag) {
      use_l_flag(files_count, regex, files, buff, pattern_count, v_flag);
    } else if (c_flag) {
      use_c_flag(files_count, regex, files, buff, pattern_count, v_flag,
                 h_flag);
    } else {
      use_eivnho_flag(files_count, regex, files, buff, pattern_count, v_flag,
                      n_flag, h_flag, o_flag);
    }
  }
  for (int i = 0; i < pattern_count; i++) {
    regfree(&regex[i]);
  }
}

void use_c_flag(int files_count, regex_t *regex, char files[][MAX_LENGTH],
                char *buff, int pattern_count, int v_flag, int h_flag) {
  int c_count = 0;
  int match = 0;

  for (int j = 0; j < files_count; j++) {
    FILE *f = fopen(files[j], "r");
    if (f != 0) {
      while ((fgets(buff, MAX_LENGTH, f)) != NULL) {
        for (int i = 0; i < pattern_count; i++) {
          if (regexec(&regex[i], buff, 0, NULL, 0) == 0) {
            match = 1;
          }
        }
        if ((v_flag && !match) || (!v_flag && match)) {
          c_count++;
        }
        match = 0;
      }
      fclose(f);
    }
    if (files_count > 1 && !h_flag) {
      printf("%s:", files[j]);
    }
    printf("%d\n", c_count);
    c_count = 0;
  }
}

void use_l_flag(int files_count, regex_t *regex, char files[][MAX_LENGTH],
                char *buff, int pattern_count, int v_flag) {
  int checker_1;
  int checker_2;
  for (int j = 0; j < files_count; j++) {
    FILE *f = fopen(files[j], "r");
    if (f != 0) {
      checker_1 = 0;
      checker_2 = 1;
      while ((fgets(buff, MAX_LENGTH, f)) != NULL) {
        for (int i = 0; i < pattern_count; i++) {
          if (regexec(&regex[i], buff, 0, NULL, 0) == 0) {
            checker_1 = 1;
          } else {
            checker_2 = 0;
          }
        }
      }
      if (checker_1 && !v_flag) {
        printf("%s\n", files[j]);
      } else if (!checker_2 && v_flag) {
        printf("%s\n", files[j]);
      }
      fclose(f);
    }
  }
}

void use_eivnho_flag(int files_count, regex_t *regex, char files[][MAX_LENGTH],
                     char *buff, int pattern_count, int v_flag, int n_flag,
                     int h_flag, int o_flag) {
  int regex_res;
  regmatch_t match[1];
  for (int j = 0; j < files_count; j++) {
    FILE *f = fopen(files[j], "r");
    if (f != NULL) {
      int line_number = 1;
      while (fgets(buff, MAX_LENGTH, f) != NULL) {
        int matched = 0;
        for (int i = 0; i < pattern_count; i++) {
          if (o_flag && !v_flag) {
            use_o_flag(match, buff, regex, files_count, h_flag, j, files,
                       &matched, n_flag, line_number, pattern_count);
            break;
          } else {
            regex_res = regexec(&regex[i], buff, 0, NULL, 0);
            if (regex_res == 0) {
              matched = 1;
              break;
            }
          }
        }
        if (o_flag && !v_flag) {
          line_number++;
          continue;
        } else if (o_flag && v_flag) {
          continue;
        }
        if ((!v_flag && matched) || (v_flag && !matched)) {
          if (files_count > 1 && h_flag == 0) {
            printf("%s:", files[j]);
          }
          if (n_flag) {
            printf("%d:", line_number);
          }
          printf("%s", buff);
          if (buff[strlen(buff) - 1] != '\n') {
            printf("\n");
          }
        }
        line_number++;
      }
      fclose(f);
    }
  }
}

int create_regular(int pattern_count, regex_t *regex, int E_flag, int i_flag,
                   char pattern[][MAX_LENGTH]) {
  int error_flag = 0;
  for (int i = 0; i < pattern_count; i++) {
    int flags = 0;
    if (E_flag) {
      flags |= REG_EXTENDED;
    }
    if (i_flag) {
      flags |= REG_ICASE;
    }
    if (regcomp(&regex[i], pattern[i], flags) != 0) {
      fprintf(stderr, "Error in compilation: %s\n", pattern[i]);
      error_flag = 1;
    }
  }
  return error_flag;
}

void use_o_flag(regmatch_t match[1], char *buff, regex_t *regex,
                int files_count, int h_flag, int file_index,
                char files[][MAX_LENGTH], int *matched, int n_flag,
                int line_number, int pattern_count) {
  Match matches[1000];
  int match_count = 0;
  for (int i = 0; i < pattern_count; i++) {
    const char *cursor = buff;
    int offset = 0;
    while (regexec(&regex[i], cursor, 1, match, 0) == 0) {
      int start = offset + match[0].rm_so;
      int end = offset + match[0].rm_eo;
      int duplicate = 0;
      for (int k = 0; k < match_count; k++) {
        if (matches[k].start == start && matches[k].end == end) {
          duplicate = 1;
        }
      }
      if (!duplicate && match_count < 1000) {
        matches[match_count].start = start;
        matches[match_count].end = end;
        match_count++;
      }
      if (match[0].rm_eo == 0) break;
      offset += match[0].rm_eo;
      cursor += match[0].rm_eo;
    }
  }
  for (int i = 0; i < match_count - 1; i++) {
    for (int j = i + 1; j < match_count; j++) {
      if (matches[i].start > matches[j].start) {
        Match tmp = matches[i];
        matches[i] = matches[j];
        matches[j] = tmp;
      }
    }
  }
  for (int i = 0; i < match_count; i++) {
    if (files_count > 1 && h_flag == 0) printf("%s:", files[file_index]);
    if (n_flag) printf("%d:", line_number);
    printf("%.*s\n", matches[i].end - matches[i].start,
           buff + matches[i].start);
  }
  if (match_count > 0) *matched = 1;
}
