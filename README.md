# Welcome to My Ls
***

## Task
For each operand that names a file of a type other than directory, my_ls displays its name as well as any requested, associated information. For each operand that names a file of type directory, my_ls displays the names of files contained within that directory, as well as any requested, associated information. If no operands are given, the contents of the current directory are displayed. If more than one operand is given, non-directory operands are displayed first; directory and non-directory operands are sorted separately and in lexicographical order.

## Description
From main, either the current dir is printed directly or, if there is more than one argument, the input parser is called. Then requested files are printed first, after that requested dirs. Inside of dirs, dirs are printed first, then files. For each dir a linked list is created (in "create_dirent_list") which is passed to the helper functions to sort alphabetically (starting from "ret_alpha_sort"). If -t flag is present, the helper functions for mtime handling are called (starting from "ret_time_sort", or "ret_time_sort_dir" if the dir name needs to be passed for filestat info): files are sorted first by their mtime, then another sorting algorithm puts entries of same mtime into alphabetical order.

## Installation
Simply use make, then run the program by typing "./my_ls"

## Usage
To print the current directory, no other arguments are needed.

The following options are available: -a Include directory entries whose names begin with a dot (.). -t Sort by time modified (most recently modified first) before sorting the operands by lexicographical order.

Any requested files or directories are arguments after "./my_ls" or after the flags if they are present. NOTE: Make sure directories end with a "/", files end without it

Examples: "./my_ls" (prints current dir), "./my_ls -a /Users/Desktop/Dir/" (prints requested dir with hidden files)