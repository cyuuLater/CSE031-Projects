#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Partner: Matthew Kum */

//structs
struct coordinateNode{
    int x;
    int y;
    int value;

    struct coordinateNode *prev;
};

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void printFinalizedPathsArr();
void searchPuzzle(char** arr, char* word);
void checkNeighbors(int xPos, int yPos, char desiredLetter, char** arr, int** pathArr, int wordIndexOffset, char* word, struct coordinateNode* path);

int pathExists();
int getNewPathArrValue(int currentValue, int valueToAdd);

struct coordinateNode *path;
struct coordinateNode *pathToMake;

int bSize;
int wordIndex;
int endSearch;
int pathCount;
int wordWasFound;

int** finalizedPathsArr;

char getAsUpper(char c){
    //if it is not an upper case letter. AKA, it's ASCII value is not from 65 to 90
    if ((97 <= (int)c) && ((int)c <= 122)){//also, checks if it is a letter, by checking if it's ASCII value is between 97 and 122 
        return (char)((int)c-32);
    }

    return c;
}

void allocateIntArray(int** p){
    for(int i = 0; i < bSize; i++) {
        *(p + i) = (int*)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; j++) {
            *(*(p + i) + j) = 0;
        }
    }
}

// Main function, DO NOT MODIFY
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
        int i, j;
        FILE *fptr;

        // Open file for reading puzzle
        fptr = fopen(argv[1], "r");
        
        if (fptr == NULL) {
            printf("Cannot Open Puzzle File!\n");
        return 0;
    }
    
    // Read the size of the puzzle block. Assume that the size is the same for length and width
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));
    
    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }

    fclose(fptr);
    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);

    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);


    return 0;
}

void printPuzzle(char** arr) {
    // This function will print out the complete puzzle grid (arr).
    // It must produce the output in the SAME format as the samples
    // in the instructions.
    // Your implementation here...
    for (int x = 0; x < bSize; x++){
        char* row = (char*)malloc(bSize * sizeof(char));//[bSize];//resets each new x loop
        int counter = 0;//resets each new x loop

        for (int y = 0; y < bSize; y++){
            if (y == bSize - 1){ //if the last letter in the row
                //do not give a space after
                *(row + counter) = *(*(arr + x) + y);
                *(row + counter + 1) = '\0';
                counter++;
            }
            else {
                *(row + counter) = *(*(arr + x) + y);
                *(row + counter + 1) = ' ';
                counter += 2;
            }
        }
        
        printf("%s", row);
        printf("\n");
    }
}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...

    wordIndex = 0;
    endSearch = 0;//means false
    wordWasFound = 0;

    //initialize pathArr
    int** pathArr = (int**)malloc(bSize * sizeof(int*));
    allocateIntArray(pathArr);

    //initialize array that holds all the finalized paths
    finalizedPathsArr = (int**)malloc(bSize * sizeof(int*));//it is global so that it can be accessed anywhere
    allocateIntArray(finalizedPathsArr);

    path = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));//starting part of the path
    path->prev = 0;
    pathCount = 0;

    //check for the word
    for (int x = 0; x < bSize; x++){
        for (int y = 0; y < bSize; y++){
            if (getAsUpper(*word) == *(*(arr + x) + y)){
                checkNeighbors(x, y, *(word + wordIndex + 1), arr, pathArr, 1, word, path);
                
                if (endSearch == 1){//if was a successful search
                    //reset endSearch and set the starting spot's value
                    *(*(finalizedPathsArr + x) + y) = getNewPathArrValue(*(*(finalizedPathsArr + x) + y), 1);
                    endSearch = 0;
                }
            }
        }
    }

    //print out finalizedPathsArr
    if (pathExists() == 1){//if there was a path found
     
        //print out finalizedPathsArr
        printf("\nWord found!\nPrinting the search path:\n");
        printFinalizedPathsArr();
    }
    else {//no path was found
        //print out message
        printf("\nWord not found!\n");
    }

    /*
     - the possible neighbors to a letter are up down left right and diagonals
     - when marking a letter, in the place it is supposed to be, instead of placing a zero which indicates it is not used, place the number that represents its placement in the word.
     - if they overlap, then put all the numbers that represent it's position with the most recent being placed to the farthest left
    */
}

/*
=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
HOW IT WORKS:
    1. check in all 8 directions
    2. if any are a match, then set the new value in pathArr
    3. once the value is set, make a new coordianteNode that holds the coordinate of the neighbor and it's value. Then set the coordinateNode's prev to path
        - this allows it to backtrack the proper path once a proper path is found
    4. recursively call the function with the neighbor's coordinates, the wordIndexOffset being 1 higher and the new coordinateNode as path
    5. once a proper path is found(i.e. it has come across \0), the program will use a while loop to go through the path, and if the spot on finalizedPathsArr is empty(it is zero), then put the value of the coordinateNode in that spot

NOTES:
    - The program needs to write to a seperate path array before writing to another one because when the function is going, it needs to account for a previous value in a spot with a suitable neighbor.
    - The program can disregard any duplicate coordinateNodes encountered after their first appearance, because it reads them backwards. Meaning the latest values of the duplicates will be encountered first. 
=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
*/

void checkNeighbors(int xPos, int yPos, char desiredLetter, char** arr, int** pathArr, int wordIndexOffset, char* word, struct coordinateNode* path){
    //the puzzle is read with (0,0) being at the top left
    if (endSearch == 0){
        //if wordIndexOffset is zero ignore it
        if (wordIndexOffset != 0){
            desiredLetter = *(word + wordIndexOffset);
        }
        desiredLetter = getAsUpper(desiredLetter);


        if (*(word + wordIndexOffset) == '\0' || *(word) == '\0'){
            //end this search and assign values to the finalized paths array
            //while there is more to the path
            while (path->prev != 0){
                if (*(*(finalizedPathsArr + path->x) + path->y) == 0)
                    *(*(finalizedPathsArr + path->x) + path->y) = path->value;

                path = path->prev;
            }

            wordWasFound = 1;//means true
            endSearch = 1;//means true
        }
        else {
            if (yPos - 1 >= 0){
                //check if it is a hit
                if (*(*(arr + xPos) + (yPos - 1)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    int newValue = getNewPathArrValue(*(*(pathArr + xPos) + (yPos - 1)), wordIndex + wordIndexOffset + 1);
                    *(*(pathArr + xPos) + (yPos - 1)) = newValue;//getNewPathArrValue(*(*(pathArr + xPos) + (yPos - 1)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos;
                    newPartToPath->y = yPos - 1;
                    newPartToPath->value = newValue;//*(*(pathArr + xPos) + (yPos - 1));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos, yPos - 1, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }
            
            if (yPos + 1 < bSize){
                //check if it is a hit
                if (*(*(arr + xPos) + (yPos + 1)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos;
                    newPartToPath->y = yPos + 1;
                    newPartToPath->value = getNewPathArrValue(*(*(pathArr + xPos) + (yPos + 1)), wordIndex + wordIndexOffset + 1);
                    newPartToPath->prev = path;

                    checkNeighbors(xPos, yPos + 1, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }

            if (xPos - 1 >= 0){
                //check if it is a hit
                if (*(*(arr + (xPos - 1)) + (yPos)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    *(*(pathArr + (xPos - 1)) + (yPos)) = getNewPathArrValue(*(*(pathArr + (xPos - 1)) + (yPos)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos - 1;
                    newPartToPath->y = yPos;
                    newPartToPath->value = *(*(pathArr + (xPos - 1)) + (yPos));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos - 1, yPos, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }

            if (xPos + 1 < bSize){
                //check if it is a hit
                if (*(*(arr + (xPos + 1)) + (yPos)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    *(*(pathArr + (xPos + 1)) + (yPos)) = getNewPathArrValue(*(*(pathArr + (xPos + 1)) + (yPos)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos + 1;
                    newPartToPath->y = yPos;
                    newPartToPath->value = *(*(pathArr + (xPos + 1)) + (yPos));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos + 1, yPos, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }

            if (xPos - 1 >= 0 && yPos - 1 >= 0){
                //check if it is a hit
                if (*(*(arr + (xPos - 1)) + (yPos - 1)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    *(*(pathArr + (xPos - 1)) + (yPos - 1)) = getNewPathArrValue(*(*(pathArr + (xPos - 1)) + (yPos - 1)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos - 1;
                    newPartToPath->y = yPos - 1;
                    newPartToPath->value = *(*(pathArr + (xPos - 1)) + (yPos - 1));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos - 1, yPos - 1, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }

            if (xPos + 1 < bSize && yPos - 1 >= 0){
                //check if it is a hit
                if (*(*(arr + (xPos + 1)) + (yPos - 1)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    *(*(pathArr + (xPos + 1)) + (yPos - 1)) = getNewPathArrValue(*(*(pathArr + (xPos + 1)) + (yPos - 1)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos + 1;
                    newPartToPath->y = yPos - 1;
                    newPartToPath->value = *(*(pathArr + (xPos + 1)) + (yPos - 1));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos + 1, yPos - 1, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }

            if (xPos - 1 >= 0 && yPos + 1 < bSize){
                //check if it is a hit
                if (*(*(arr + (xPos - 1)) + (yPos + 1)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    *(*(pathArr + (xPos - 1)) + (yPos + 1)) = getNewPathArrValue(*(*(pathArr + (xPos - 1)) + (yPos + 1)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos - 1;
                    newPartToPath->y = yPos + 1;
                    newPartToPath->value = *(*(pathArr + (xPos - 1)) + (yPos + 1));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos - 1, yPos + 1, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }

            if (xPos + 1 < bSize && yPos + 1 < bSize){
                //check if it is a hit
                if (*(*(arr + (xPos + 1)) + (yPos + 1)) == desiredLetter){
                    //assign pathArr at the coords to be the new thing. Then call this function again and send in wordIndexOffset as one bigger and the neighbor's coords
                    *(*(pathArr + (xPos + 1)) + (yPos + 1)) = getNewPathArrValue(*(*(pathArr + (xPos + 1)) + (yPos + 1)), wordIndex + wordIndexOffset + 1);
                    struct coordinateNode *newPartToPath = (struct coordinateNode*) malloc(sizeof(struct coordinateNode));
                    newPartToPath->x = xPos + 1;
                    newPartToPath->y = yPos + 1;
                    newPartToPath->value = *(*(pathArr + (xPos + 1)) + (yPos + 1));
                    newPartToPath->prev = path;

                    checkNeighbors(xPos + 1, yPos + 1, desiredLetter, arr, pathArr, wordIndexOffset + 1, word, newPartToPath);
                }
            }
        }
    }
}

//currentValue is the value at the spot in pathArr, and valueToAdd is the value to be prepended to currentValue
int getNewPathArrValue(int currentValue, int valueToAdd){
    int tenMultiplier = 10;

    //if currentValue is equal to zero
    if (currentValue == 0){
        return valueToAdd;
    }
    else {
        //go through and check how how many spaces it has taken up. i.e. 21 would mean multiply ten Multiplier by ten once, so that can have 100
        while (currentValue / tenMultiplier != 0){//while the currentValue divided by tenMultiplier is possible, mulitply thetenMultiplier variable by itself
            tenMultiplier *= 10;
        }

        return currentValue + (valueToAdd * tenMultiplier);
    }
}

int pathExists(){
    return wordWasFound;//wordWasFound can only be 0 or 1. 1 means true, and in this program, it also means a proper path was found
}

int getNumberOfSpacesToAdd(int defaultAmount, int valueToAdjustBasedOn){
    int spacesOccupied = 1;
    int multiplier = 10;

    if (valueToAdjustBasedOn == 0)
        return defaultAmount;

    while (valueToAdjustBasedOn / multiplier != 0){
        multiplier *= 10;
        spacesOccupied++;
    }

    return (defaultAmount - (spacesOccupied - 1));//spacesOccupied minus 1 because if only one space is occupied then no need to reduce spaces to add 
}

void printFinalizedPathsArr(){
    for (int x = 0; x < bSize; x++){
        for (int y = 0; y < bSize; y++){
            //get how many spaces to add
            int numOfSpaces = getNumberOfSpacesToAdd(bSize + 2, *(*(finalizedPathsArr + x) + y));

            //print the value and then add in the required number of spaces
            printf("%i", *(*(finalizedPathsArr + x) + y));
            for (int spaces = 0; spaces < numOfSpaces; spaces++){
                printf(" ");
            }         
        }

        printf("\n");
    }
}