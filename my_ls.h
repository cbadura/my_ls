#ifndef MY_LS_H
#define MY_LS_H

#include <stdbool.h>

#ifndef STRUCT_LISTNODE
#define STRUCT_LISTNODE
typedef struct s_listnode {
    char *str;
    bool dir;
    long long mtime;
    struct s_listnode* next;
} lnode;
#endif

#ifndef STRUCT_INPUT
#define STRUCT_INPUT
typedef struct s_input {
    bool a;
    bool t;
    bool dir;
    int size;
    char **args;
} t_input;
#endif

#ifndef ENUM_FILEMODE
#define ENUM_FILEMODE
typedef enum t_filemode {
    FILES,
    DIRS,
    ALL
} filemode;
#endif

t_input parsing(char **argv);
t_input fill_struct(t_input input, char **argv);
// int not_just_dots(char *str);

lnode *ret_time_sort(lnode *head);
lnode *ret_time_sort_dir(lnode *head, char *dir);
lnode *sort_same_time_alpha(lnode *head);
lnode *sort_mtime(lnode *head);
void fill_mtime(lnode *head);
void fill_mtime_dir(lnode *head, char *dir);
long long check_mtime(char *file);
long long check_mtime_dir(char *file, char *dir);
void time_swap(lnode *node, lnode *node2);
void bool_swap(lnode *node, lnode *node2);

lnode *ret_alpha_sort(lnode *head);
lnode *bubble_sort_alpha(lnode *head, int pos);
int longest_str_in_list(lnode *head);
int sort(lnode *node, int pos, int swapped);
void swap(lnode *node, lnode *node2);

lnode *new_elem_dir(lnode *head, char *str, bool dir);
lnode *new_elem_time(lnode *head, long long time);
void print_list(lnode *head);
void deallocate(lnode *head);

int my_strlen(char *string);
char* my_strcpy(char *dst, char *src);
int my_strcmp(char *s1, char *s2);
char *my_strcat(char *dest, char *src);
char *my_strdup(char* str);

#endif

