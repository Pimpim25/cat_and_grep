#ifndef CHECK
#define CHECK
int check_file_exist(char *file, char *current_flags, int type);
#endif

#ifndef SORT
#define SORT
int sort_short_flags(char *user_flags, char *current_flags, char *static_flags,
                     int type);
#endif