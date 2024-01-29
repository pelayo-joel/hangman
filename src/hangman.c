#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <ncurses/ncurses.h>


#include "../include/hangman.h"
#include "../include/file_read.h"



char* dictionnary = NULL;
char* category = NULL; 
char* difficulty = NULL;
char* wordToGuess = NULL;
char* wordCategory = NULL;



int main(int argc, char* argv[]) {
    initscr();
	clear();

	//Parses the cmd arguments
	if (argc == 4) {
		dictionnary = argv[1];
		category = argv[3];
		difficulty = argv[2];
	}
	else if (argc == 3) {
		dictionnary = argv[1];
		category = "all";
		difficulty = argv[2];
	}
	else if (argc == 1) {
		dictionnary = "dictionnary.csv";
		category = "all";
		difficulty = "all";
	}
	else {
		printf("Invalid use of arguments\n");
		return 1;
	}

    //Retrieves corresponding word
	GetWord();

    //Game start
    char guessedLetters[MAX_WORD_LENGTH];
    int tries = 0;

    // Initialize ncurses
    raw();
    keypad(stdscr, TRUE);
    noecho();
    clear();

    // Convert the word to uppercase
    for (size_t i = 0; i < strlen(wordToGuess); ++i) {
        wordToGuess[i] = toupper(wordToGuess[i]);
    }

    // Initialize guessed letters
    memset(guessedLetters, 0, sizeof(guessedLetters));

    // Main game loop
    while (!isGameOver(wordToGuess, guessedLetters) && tries < MAX_TRIES) {
        char guess;
        clear();
        printw("CHANCES LEFT: %i\n", (MAX_TRIES - tries));

        // Draw hangman and word
        drawHangman(tries);
        drawWord(wordToGuess, guessedLetters);
        // drawPressedCharacters(guess);

        // Get a guess from the player
        printw("\n\t  Category: %s", wordCategory);
        printw("\n\n\t  Press a letter");
        // guess[tries] = getch();
        guess = getch();
        refresh();

        // Convert the guess to uppercase
        // guess[tries] = toupper(guess[tries]);
        guess = toupper(guess);

        // Check the guess
        if (!checkGuess(guess, wordToGuess, guessedLetters)) {
            tries++;
        }
    }

    // Display the final result
    clear();
    drawHangman(tries);
    drawWord(wordToGuess, guessedLetters);

    if (tries >= MAX_TRIES) {
        printw("\n\n\t  You lose! The word was: %s\n", wordToGuess);
    } else {
        printw("\n\n\t  Congratulations! You guessed the word: %s\n", wordToGuess);
    }

    refresh();
    getch();

    // End ncurses
    endwin();

    return 0;
}



//Retrieves the words from the dictionnary depending on the given args as a linked list and chooses a random word in the list
void GetWord() {
    printw("Reading from %s...", dictionnary);
	node possibleWords = matching_words(dictionnary, category, difficulty);
	wordToGuess = malloc(sizeof(char) * 32);
	wordCategory = malloc(sizeof(char) * 32);

	srand(time(NULL));
	int random = rand() % listLen(possibleWords);

	int i = 0;
	while (possibleWords) {
		if (i == random) {
			strcpy(wordToGuess, possibleWords->wordData[0]);
			strcpy(wordCategory, possibleWords->wordData[1]);
		}
		destroy(possibleWords);
		possibleWords = possibleWords->next;
		i++;
	}
	getch();
}

void drawHangman(int tries) {
    // Draw hangman based on the number of incorrect guesses
    mvprintw(2, 10, "+----+");
    mvprintw(3, 10, "|/   |");
    mvprintw(4, 10, "|");
    mvprintw(5, 10, "|");
    mvprintw(6, 10, "|");
    mvprintw(7, 10, "|");
    mvprintw(8, 10, "+");

    switch (tries) {
        case 1:
            mvprintw(4, 15, "O");
            break;
        case 2:
			mvprintw(4, 15, "O");
            mvprintw(5, 15, "|");
            break;
        case 3:
			mvprintw(4, 15, "O");
            mvprintw(5, 15, "|");
            mvprintw(5, 14, "/");
            break;
        case 4:
			mvprintw(4, 15, "O");
            mvprintw(5, 15, "|");
            mvprintw(5, 14, "/");
            mvprintw(5, 16, "\\");
            break;
        case 5:
			mvprintw(4, 15, "O");
            mvprintw(5, 15, "|");
            mvprintw(5, 14, "/");
            mvprintw(5, 16, "\\");
            mvprintw(6, 14, "/");
            break;
        case 6:
			mvprintw(4, 15, "O");
            mvprintw(5, 15, "|");
            mvprintw(5, 14, "/");
            mvprintw(5, 16, "\\");
            mvprintw(6, 14, "/");
            mvprintw(6, 16, "\\");
            break;
        default:
            break;
    }

    refresh();
}

void drawWord(const char *word, const char *guessedLetters) {
    // Display the word with correctly guessed letters
    mvprintw(12, 10, "Word: ");
    for (size_t i = 0; i < strlen(word); ++i) {
        if (guessedLetters[i] == 1 || word[i] == ' ') {
            mvprintw(12, 15 + i * 2, "%c", word[i]);
        } else {
            mvprintw(12, 15 + i * 2, "_");
        }
    }

    refresh();
}

void drawPressedCharacters(const char *pressedChar) {
    // Display pressed characters
    mvprintw(13, 10, "Pressed letters: ");
    for (int i = 0; i < MAX_TRIES; ++i) {
        mvprintw(13, 28 + i * 2, "%c", pressedChar[i]);
    }

    refresh();
}

int checkGuess(char guess, const char *word, char *guessedLetters) {
    // Check if the guessed letter is in the word
    int correct = 0;
    for (size_t i = 0; i < strlen(word); ++i) {
        if (word[i] == guess) {
            guessedLetters[i] = 1;
            correct = 1;
        }
    }

    return correct;
}

int isGameOver(const char *word, const char *guessedLetters) {
    // Check if the game is over (all letters guessed or maximum tries reached)
    for (size_t i = 0; i < strlen(word); ++i) {
        if (!guessedLetters[i] && word[i] != ' ') {
            return 0; // Game is not over
        }
    }

    return 1; // Game is over
}

int listLen(node listHead) {
	int len = 0;
	while (listHead) {
		listHead = listHead->next;
		len++;
	}
	return len;
}