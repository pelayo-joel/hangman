#ifndef HANGMAN_H
#define HANGMAN_H

#include "../include/file_read.h"

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6



// int i_rnd(int i);

// char* decrypt(char* code);

void drawHangman(int tries);

void drawWord(const char *word, const char *guessedLetters);

void drawPressedCharacters(const char *pressedChar);

int checkGuess(char guess, const char *word, char *guessedLetters);

int isGameOver(const char *word, const char *guessedLetters);

int listLen(node listHead);

#endif