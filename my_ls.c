#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include "my_ls.h"

lnode *create_dirent_list(t_input input, int index, int filemode);
void dirent_loop(t_input input);
void print_curr_dir(t_input input);
void print_files(t_input input);

int main(int argc, char **argv)
{
    // argument handling
    t_input input = parsing(argv);

    if (argc == 1)
    {
        print_curr_dir(input);
        return 0;
    }

    if (argc > 1)
    {
        if ((my_strcmp(argv[argc-1], ".") == 0) || argv[argc-1][0] == '-')
        {
            print_curr_dir(input);
            return 0;
        }
        else
            input = parsing(argv);
    }

    print_files(input);
    dirent_loop(input);

    free(input.args);
    return 0;
}

void print_curr_dir(t_input input)
{
    lnode *head = NULL;
    input.args[0] = ".";

    printf("*** REQUESTED DIR: . *** \n");
    if (input.t)
    {
        head = create_dirent_list(input, 0, ALL);
        head = ret_time_sort(head);
        print_list(head);
    }
    else
    {
        head = create_dirent_list(input, 0, DIRS);
        head = ret_alpha_sort(head);
        print_list(head);

        head = create_dirent_list(input, 0, FILES);
        head = ret_alpha_sort(head);
        print_list(head);
    }
    deallocate(head);
}

void dirent_loop(t_input input)
{
    lnode *head = NULL;
    int len = 0;

    for (int i = 0; input.args[i]; i++)
    {
        len = my_strlen(input.args[i]);
        if (input.args[i][len-1] == '/' /* || (my_strcmp(input.args[i], ".") == 0) */)
        {
            printf("*** REQUESTED DIR: %s *** \n", input.args[i]);
            if (input.t)
            {
                head = create_dirent_list(input, i, ALL);
                head = ret_time_sort_dir(head, input.args[i]);
                print_list(head);
            }
            else
            {
                head = create_dirent_list(input, i, DIRS);
                head = ret_alpha_sort(head);
                print_list(head);

                head = create_dirent_list(input, i, FILES);
                head = ret_alpha_sort(head);
                print_list(head);
            }
        }
    }
    // deallocate(head);
}

void print_files(t_input input)
{
    struct stat filestat;
    lnode *head = NULL;
    int len = 0; // for checking the '/' at the end of args

    printf("*** REQUESTED FILES: ***\n");
    for (int i = 0; input.args[i]; i++)
    {
        if ((my_strcmp(input.args[i], ".") == 0)) continue;
        // exclude files starting with '.', include only if -a flag is present
        if (!input.a)
            if (input.args[i][0] == '.') continue;
        
        // check if arg is folder by checking for '/', print only files
        len = my_strlen(input.args[i]);
        if (input.args[i][len-1] != '/')
        {
            if (lstat(input.args[i], &filestat) == -1)
            {
                printf("Requested file %s doesn't exist!\n", input.args[i]);
                continue;
            }
            head = new_elem_dir(head, input.args[i], false);
        }
    }

    if (input.t)
        head = ret_time_sort(head);
    else
        head = ret_alpha_sort(head);

    print_list(head);
    if (head) deallocate(head);
}

lnode *create_dirent_list(t_input input, int index, int filemode)
{
    lnode *head = NULL;
    struct dirent *de;
    DIR *dirp = opendir(input.args[index]);
    if (dirp == NULL)
    {
        printf("Requested directory %s doesn't exist!\n", input.args[index]);
        return NULL;
    }
  
    char *entry = NULL;
    while ((de = readdir(dirp)) != NULL)
    {
        if (!input.a)
            if (de->d_name[0] == '.') continue;

        // create list
        if (de->d_type == DT_REG && filemode == FILES)
        {
            entry = my_strdup(de->d_name);
            head = new_elem_dir(head, entry, false);
        }
        else if (de->d_type == DT_DIR && filemode == DIRS)
        {
            entry = my_strdup(de->d_name);
            char *dirName = calloc((my_strlen(entry) + 2), sizeof(char));
            my_strcat(dirName, entry);
            my_strcat(dirName, "/");
            head = new_elem_dir(head, dirName, true);
            // free(entry);
        }
        else if (filemode == ALL)
        {
            if (de->d_type == DT_REG)
            {
                entry = my_strdup(de->d_name);
                head = new_elem_dir(head, entry, false);
            }
            if (de->d_type == DT_DIR)
            {
                entry = my_strdup(de->d_name);
                char *dirName = calloc((my_strlen(entry) + 2), sizeof(char));
                my_strcat(dirName, entry);
                my_strcat(dirName, "/");
                head = new_elem_dir(head, dirName, true);
            }
        }
    }
    closedir(dirp);
    return head;
}

// print in red: printf("\033[31m %s \033[0m", filename); // part after "%s" to print only this item in red