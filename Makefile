
my_tar:
	gcc -Wall -Wextra -Werror -o my_tar my_tar.c

my_tar.o: my_tar.c
	gcc -Wall -Wextra -Werror -c my_tar.c

clean:
	rm my_tar

fclean: clean
	rm my_tar

re: fclean my_tar