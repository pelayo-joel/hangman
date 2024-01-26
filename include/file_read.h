#ifndef FILE_READ_H
#define FILE_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct word* node;

struct word {
    char* wordData[3]; 
    node next;
};

node file_words(char* filePath);

node matching_words(char* filePath, char* category, char* diffculty);

node add_word(node listStart, char** wordData);




#endif