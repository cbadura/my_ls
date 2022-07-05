SOURCES = my_ls.c my_ls_helpers.c string_mgmt.c
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -Wall -Wextra -Werror -O2 --pedantic -g3 -fsanitize=address

my_ls: $(OBJECTS)
	cc $(CFLAGS) -o my_ls $(OBJECTS)

clean:
	rm -rf $(OBJECTS)

fclean:
	rm -f my_ls clean