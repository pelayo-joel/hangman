#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/file_read.h"


#define MAXCHAR 1024



//Retrieves all words that matches what args has been given by the user
node matching_words(char* filePath, char* category, char* difficulty) {
    node allWords = file_words(filePath);

    //If no args has been given
    if (!strcmp(category, "all") && !strcmp(difficulty, "all"))
    {
        return allWords;
    }
    node startPtr = NULL;
    node currentWord = allWords;

    //Traverse all retrieved word from the file
    while (currentWord && currentWord->next != NULL)
    {
        //If matches both arguments
        if (strcmp(currentWord->wordData[1], category) == 0 && strcmp(currentWord->wordData[2], difficulty) == 0)
        {
            startPtr = add_word(startPtr, currentWord->wordData);
        }
        // If only two args has been given and match on diffculty
        else if (strcmp(category, "all") == 0 && strcmp(currentWord->wordData[2], difficulty) == 0)
        {
            startPtr = add_word(startPtr, currentWord->wordData);
        }
        
        currentWord = currentWord->next;
    }
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
    while (fgets(row, MAXCHAR, file))
    {
        int parsingErrorFlag = 0;
        int column = 0;
        char* currentWord[3];
        token = strtok(row, ",");
        
        //Row is a comment
        if (token[0] == '#') continue;

        //Traverse the row
        while (token)
        {
            currentWord[column] = token;
            //If word in file is incorrect
            if ((column == 1 && strcmp(token, "fruits") != 0 && strcmp(token, "langages") != 0) || (column == 2 && strcmp(token, "facile\n") != 0 && strcmp(token, "moyen\n") != 0 && strcmp(token, "difficile\n") != 0))
            {
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
    // Traverse the list until we reach the end then adds it to the tail of the list
    else {
        node lastWord = listStart;
        while (lastWord->next != NULL)
        {
            lastWord = lastWord->next;
        }
        lastWord->next = newWord;
    }
    return listStart;
}