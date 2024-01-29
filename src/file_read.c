#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../include/file_read.h"

#define N_CATEGORIES 2
#define N_DIFFICULTIES 3

char* validCategories[N_CATEGORIES] = {"fruits", "langages"};
char* validDifficulties[N_DIFFICULTIES] = {"facile\n", "moyen\n", "difficile\n"};


//Retrieves all words that matches what args has been given by the user
node matching_words(char* filePath, char* category, char* difficulty) {
    node allWords = file_words(filePath);

    //If no args has been given
    if (!strcmp(category, "all") && !strcmp(difficulty, "all")) {
        return allWords;
    }
    node startPtr = NULL;
    node currentWord = allWords;

    //Traverse all retrieved word from the file
    while (currentWord && currentWord->next != NULL) {
        //If matches both arguments or only two args has been given and match on diffculty
        if ((strcmp(currentWord->wordData[1], category) == 0 && strcmp(currentWord->wordData[2], difficulty) == 0) || (strcmp(category, "all") == 0 && strcmp(currentWord->wordData[2], difficulty) == 0)) {
            startPtr = add_word(startPtr, currentWord->wordData);
        }
        currentWord = currentWord->next;
    }
    destroy(allWords);
    return startPtr;
}

//Retrieves all words and their properties from the file 
node file_words(char* filePath) {   
    //Opens the file and creates a buffer
    FILE* file = fopen(filePath, "r");
    char row[MAXCHAR]; 
    char* token;

    //Initialize the head of the list
    node startPtr = NULL;

    //Traverse the file row by row
    int nRow = 0;
    while (fgets(row, MAXCHAR, file)) {
        int parsingErrorFlag = 0;
        int column = 0;
        char* currentWord[3];
        token = strtok(row, ",");
        
        //Row is a comment
        if (token[0] == '#') continue;

        //Traverse the row
        while (token) {
            
            currentWord[column] = token;
            //If category or difficulty is invalid
            if ((column == 1 && valid_word(token, validCategories, N_CATEGORIES) == 1) || (column == 2 && valid_word(token, validDifficulties, N_DIFFICULTIES) == 1)) {
                printf("Error on line %i: %s\n", nRow, token);
                parsingErrorFlag = 1;
                break;
            }
            token = strtok(NULL, ",");
            column++;
        }
        if (parsingErrorFlag == 1) continue;

        startPtr = add_word(startPtr, currentWord);
        nRow++;
    }

    fclose(file);
    return startPtr;
}

//Adds a new word in the linked list
node add_word(node listStart, char** wordData) {
    //Trim the difficulty if it directly comes from the file
    int diffLen = strlen(wordData[2]);
    char* trimDiff = wordData[2];
    if (trimDiff[diffLen - 1] == '\n') trimDiff[diffLen - 1] = '\0';

    //Creates a new word
    node newWord = (node) malloc(sizeof(struct word));
    newWord->wordData[0] = malloc(sizeof(char) * 32);
    newWord->wordData[1] = malloc(sizeof(char) * 32);
    newWord->wordData[2] = malloc(sizeof(char) * 32);
    
    //Inserts data into the newly created word
    strcpy(newWord->wordData[0], wordData[0]);
    strcpy(newWord->wordData[1], wordData[1]);
    strcpy(newWord->wordData[2], trimDiff);
    newWord->next = NULL;

    // If linked list doesn't exist (means that it's the first word)
    if (listStart == NULL) {
        listStart = newWord;
    }
    // Else, traverse the list until we reach the tail then adds the newly created word at it
    else {
        node lastWord = listStart;
        while (lastWord->next != NULL) {
            lastWord = lastWord->next;
        }
        lastWord->next = newWord;
    }
    return listStart;
}

//Checks if word properties are valid (valid category/difficulty and only alphabetic char)
int valid_word(char* wordProperty, char** validProperties, int nProp) {
    int invalidFlag = 0;
    for (int i = 0; i < nProp; i++){
        if (strcmp(wordProperty, validProperties[i]) == 0) {
            invalidFlag = 1;
        }   
    }
    if (invalidFlag == 0) return 1;

    for (int i = 0; wordProperty[i] < '\0'; i++) {
        if (wordProperty[i] == '-') {
            continue;
        }
        else if (!isalpha(wordProperty[i]) || !isalpha(wordProperty[i])) {
            return 1;
        }   
    }
    
    return 0;
}

void destroy(node listStart) {
	free(listStart->wordData[0]);
	free(listStart->wordData[1]);
	free(listStart->wordData[2]);
	free(listStart);
}