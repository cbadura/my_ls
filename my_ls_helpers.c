#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "my_ls.h"

/* 
*** utility functions ***
 */

t_input parsing(char **argv)
{
    t_input input;
    input.a = false;
    input.t = false;
    
    // find nbr of file/dir names in argv
    int argSize = 0;
    for (int i = 1; argv[i]; i++)
        if (argv[i][0] != '-') argSize++;

    input.args = malloc(sizeof(char*) * (argSize + 1));
    input.size = argSize;
    input = fill_struct(input, argv);
    
    if (input.args[0] == NULL) input.args[0] = ".";
    return input;
}

t_input fill_struct(t_input input, char **argv)
{
    int currArg = 0;
    for (int i = 1; argv[i]; i++)
    {
        if (my_strcmp(argv[i], "-a") == 0)
            input.a = true;

        else if (my_strcmp(argv[i], "-t") == 0)
            input.t = true;
            
        else if ((my_strcmp(argv[i], "-at") == 0) || (my_strcmp(argv[i], "-ta") == 0))
        {
            input.a = true;
            input.t = true;
        }
        else
        {
            input.args[currArg] = malloc(sizeof(char) * my_strlen(argv[i]));
            input.args[currArg] = argv[i];
            currArg++;
        }
    }
    input.args[currArg] = NULL;
    return input;
}

/* int not_just_dots(char *str)
{
    int check = 0; 
    check = my_strcmp(str, ".") != 0 && my_strcmp(str, "..") != 0;
    return check;  // 0 if just dots, 1 if not just dots
} */

/* 
*** time sort functions ***
 */

lnode *ret_time_sort(lnode *head)
{
    // fill mtime field of each lnode, sort according to mtime, then take care of same time entries
    if (head != NULL)
    {
        fill_mtime(head);
        head = sort_mtime(head);
        sort_same_time_alpha(head);
    }
    return head;
}

lnode *ret_time_sort_dir(lnode *head, char *dir)  // extra "dir" functions are needed for passing the full filepath to lstat (if not current dir)
{
    // fill mtime field of each lnode, sort according to mtime, then take care of same time entries
    if (head != NULL)
    {
        fill_mtime_dir(head, dir);
        head = sort_mtime(head);
        printf("sorted:\n");
        print_list(head);
        sort_same_time_alpha(head);
    }
    return head;
}

lnode *sort_same_time_alpha(lnode *head)
{
    lnode *timeList = head;
    lnode *first = head;
    char *tmp;

    // bubble sort: two entries of same mtime are sorted alphabetically
    while (head->next != NULL)
    {
        if ((head->next->mtime <= head->mtime) && (my_strcmp(head->str, head->next->str) < 0))
        {
            tmp = my_strdup(head->next->str);
            head->next->str = head->str;
            head->str = tmp;
            bool_swap(head, head->next);
            head = first;
            continue;
        }
        head = head->next;
    }
    // if (tmp[0] != '\0') free(tmp);
    return timeList;
}

lnode *sort_mtime(lnode *head)
{
    int swapped;
    lnode *first;
    lnode *last = NULL;
    if (head == NULL) return head;

    do {
        swapped = 0;
        first = head;
        while (first->next != last)
        {
            // printf("time: %lld - nexttime: %lld\n", first->mtime, first->next->mtime);
            if ((first->mtime < first->next->mtime))
            {
                time_swap(first, first->next);
                swapped = 1;
            }
            first = first->next;
        }
        last = first;
        // print_list(head);
    }
    while (swapped);
    return head;
}

void fill_mtime(lnode *head)
{
    while (head != NULL)
    {
        head->mtime = check_mtime(head->str);
        head = head->next;
    }
}

void fill_mtime_dir(lnode *head, char *dir)
{
    while (head != NULL)
    {
        head->mtime = check_mtime_dir(head->str, dir);
        head = head->next;
    }
}

long long check_mtime(char *file)
{
    struct stat filestat;
    if (lstat(file, &filestat) == -1)
    {
        printf("Could not use stat\n");
        return 0;
    }
    // printf("time: %f\n", (filestat.st_mtimespec.tv_sec*1e+9) + (filestat.st_mtimespec.tv_nsec));
    // printf("time: %f\n", (filestat.st_mtim.tv_sec*1e+9) + filestat.st_mtim.tv_nsec));

    return (filestat.st_mtimespec.tv_sec);
    // return((filestat.st_mtim.tv_sec*1e+9) + filestat.st_mtim.tv_nsec);
}

long long check_mtime_dir(char *file, char *dir)
{
    char *path = malloc(sizeof(char) * (my_strlen(file) + my_strlen(dir) + 1));
    my_strcpy(path, dir);
    my_strcat(path, file);

    struct stat filestat;
    if (lstat(path, &filestat) == -1)
    {
        printf("Could not use stat\n");
        return -1;
    }
    // printf("time: %f\n", (filestat.st_mtimespec.tv_sec*1e+9) + (filestat.st_mtimespec.tv_nsec));
    // printf("time: %f\n", (filestat.st_mtim.tv_sec*1e+9) + filestat.st_mtim.tv_nsec));

    free(path);
    return filestat.st_mtimespec.tv_sec;
    // return((filestat.st_mtim.tv_sec*1e+9) + filestat.st_mtim.tv_nsec);
}

void time_swap(lnode *node, lnode *node2)
{
    long long temp = node->mtime;
    node->mtime = node2->mtime;
    node2->mtime = temp;
}

void bool_swap(lnode *node, lnode *node2)
{
    if (node->dir == true)
        node->dir = false;
    else if (node->dir == false)
        node->dir = true;

    if (node->next->dir == true)
        node->next->dir = false;
    else if (node->next->dir == false)
        node->next->dir = true;
}

/* 
*** alpha sort functions ***
 */

lnode *ret_alpha_sort(lnode *head)
{
    int longest = longest_str_in_list(head);

    for (int i = 0; i < longest; i++)
        head = bubble_sort_alpha(head, i);

    return head;
}

int longest_str_in_list(lnode *head)
{
    int len = 0;
    while (head)
    {
        if ((my_strlen(head->str)) > len)
            len = (my_strlen(head->str));
        head = head->next;
    }
    return len;
}

lnode *bubble_sort_alpha(lnode *head, int pos)
{
    int swapped;
    lnode *first;
    lnode *last = NULL;
    if (head == NULL) return head;

    do {
        swapped = 0;
        first = head;
        while (first->next != last)
        {
            if (pos > 0)
            {
                if (my_strlen(first->str) >= pos && my_strlen(first->next->str) >= pos)
                {
                    if (first->str[pos-1] == (first->next->str[pos-1]))
                        swapped = sort(first, pos, swapped);
                }
            }
            else swapped = sort(first, pos, swapped);
            first = first->next;
        }
        last = first;
    }
    while (swapped);
    return head;
}

int sort(lnode *node, int pos, int swapped)
{
    if ((node->str[pos] > node->next->str[pos]))
    { 
        swap(node, node->next);
        swapped = 1;
    }
    return swapped;
}

void swap(lnode *node, lnode *node2)
{
    char *temp = node->str;
    node->str = node2->str;
    node2->str = temp;
}

/* 
*** llist functions ***
 */

lnode *new_elem_dir(lnode *head, char *str, bool dir)
{
    lnode *new = malloc(sizeof(lnode));
    lnode *tmp = head;

    new->next = NULL;
    new->str = str;
    if (dir == true) new->dir = true;
    if (!head)
        return new;

    while (head->next)      
        head = head->next;  

    head->next = new;
    return tmp;
}

lnode *new_elem_time(lnode *head, long long time)
{
    lnode *new = malloc(sizeof(lnode));
    lnode *tmp = head;

    new->next = NULL;
    new->mtime = time;
    if (!head)
        return new;

    while (head->next)      
        head = head->next;  

    head->next = new;
    return tmp;
}

void print_list(lnode *head)
{
    lnode *current = head;
    while (current != NULL)
    {
        printf("%lld\n", current->mtime);
        if (current->dir == true)
            printf("\033[36m%s\033[0m\n", current->str);
        else
            printf("%s\n", current->str);
        current = current->next;
    }
}

void deallocate(lnode *head)
{   
    lnode *curr = head;
    while (curr->next != NULL)
    {
        lnode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    head = NULL;
}