CC := gcc
SRC := ./src/*.c
CFLAGS := -Wall -Wextra -Werror

ifeq ($(OS), Windows_NT)
    LDFLAGS += -lncursesw
else
    LDFLAGS += -lncurses
endif

all:
	$(CC) $(CFLAGS) $(SRC) -o hangman $(LDFLAGS) -DNCURSES_STATIC 


.PHONY: all clean fclean run