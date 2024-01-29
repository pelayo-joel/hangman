#ifndef HANGMAN_H
#define HANGMAN_H

#include "../include/file_read.h"

#define WORDS 10
#define WORDLEN 40
#define CHANCE 6



// int i_rnd(int i);

// char* decrypt(char* code);

void printBody(int mistakes, char* body);

void printWord(char* guess, int len);

void drawHangman(int tries);

void drawWord(const char *word, const char *guessedLetters);

int checkGuess(char guess, const char *word, char *guessedLetters);

int isGameOver(const char *word, const char *guessedLetters);

int listLen(node listHead);

#endif