CC := gcc
INC := include/
SRC := src/*.c
CFLAGS := -Wall -Werror -Wextra


all:
	$(CC) -o hangman $(SRC) $(CFLAGS)

# hangman.exe:
# 	$(CC) -o hangman $(SRC) $(CFLAGS)


.PHONY: all clean fclean run