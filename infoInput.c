#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "infoInput.h"
#include "strcut.h"



// чтение файла с описанием
void readDescription(char *file, char ** alfabet, char *voidSymbol, int *numOfTapes, int* curState, struct listOfStates *states) {
    //открываем файл
    FILE *input;
    input = fopen(file, "r");
    if (input == NULL) {
        printf("Error: file \"%s\" can't be open", file);
        exit(7);
    }

    char tmpLine[2000000];
    //читаем кол-во лент
    fgets(tmpLine, sizeof(tmpLine), input);
    *numOfTapes = strtol(tmpLine, NULL, 0);
    if (*numOfTapes == 0) {
        printf("Error: wrong number of tapes.");
        exit(8);
    }


    //читаем алфавит
    fgets(tmpLine, sizeof(tmpLine), input);
    *alfabet = strdup(tmpLine);


    //считываем пустой символ
    fgets(tmpLine, sizeof(tmpLine), input);
    *voidSymbol = tmpLine[0];

    //считываем начальное состояние
    fgets(tmpLine, sizeof(tmpLine), input);
    *curState = strtol(strcut(tmpLine, 1, strlen(tmpLine)), NULL, 0);
    if (*curState == 0) {
        printf("Error: wrong start state.");
        exit(9);
    }


    //считываем состояния
    while (fgets(tmpLine, sizeof(tmpLine), input)) {
        splitState(tmpLine, states);
    }

    fclose(input);
}


// q1: 1,2->1,L;2,R->q3;
void splitState(char state[], struct listOfStates *states) {

    struct listOfStates *newListOfStatesCell = (states->thisState == NULL) ? states :
                                               (struct listOfStates *) malloc(sizeof(*newListOfStatesCell));
    if (newListOfStatesCell == NULL) {
        printf("Error: not enough memory.\n");
        exit(1);
    }
    newListOfStatesCell->nextState = NULL;

    //определяем номер состояния
    if (state[strlen(state)] == '\n') state = strcut(state, 0, strlen(state) - 1); // избавляемся от переноса строки
    char *statePart = strdup(state);
    if (statePart == NULL) {
        printf("Error: not enough memory.\n");
        exit(1);
    }
    statePart = strtok(statePart, " ");
    state = strcut(state, strlen(statePart) + 1, strlen(state));
    newListOfStatesCell->stateNumb = strtol(strcut(statePart, 1, strlen(statePart) - 1), NULL, 0);

    struct state *prevStateCell = NULL;
    strcpy(statePart, state);
    statePart = strtok(statePart, " ");
    state = strcut(state, strlen(statePart) + 1, strlen(state));
    while (statePart != NULL) {
        struct state *newStateCell = (struct state *) malloc(sizeof(*newStateCell));
        if (newStateCell == NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        if (prevStateCell == NULL) newListOfStatesCell->thisState = newStateCell;
        else prevStateCell->next = newStateCell;


        newStateCell->curCondition = strdup(strtok(statePart, "->"));
        newStateCell->newCondition = strdup(strtok(NULL, "->"));
        if (newStateCell->curCondition == NULL || newStateCell->newCondition == NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        char *newState = strtok(NULL, "->");
        newState = strcut(newState, 1, strlen(newState) - 1);
        newStateCell->nextState = strtol(newState, NULL, 0);
        newStateCell->next = NULL;

        prevStateCell = newStateCell;
        strcpy(statePart, state);
        statePart = strtok(statePart, " ");
        if (statePart != NULL) state = strcut(state, strlen(statePart) + 1, strlen(state));
    }

    //записываем полученное состояние в конец списка состояний
    if (newListOfStatesCell != states) {
        struct listOfStates *curListOfStates = states;
        while (curListOfStates->nextState != NULL) curListOfStates = curListOfStates->nextState;
        curListOfStates->nextState = newListOfStatesCell;
    }

    free(statePart);
}



void readTapes(char *fileName, char voidSymbol, int numOfTapes, struct listOfTapes **tapes) {
    FILE *input;
    input = fopen(fileName, "r");
    if (input == NULL) {
        printf("Error: file \"%s\" can't be open.\n", fileName);
        exit(7);
    }

    //создаём нужное кол-во лент
    for (int i = 0; i < numOfTapes ; ++i) {
        //создаём пустую ленту с одним пустым символом
        struct listOfTapes *newListOfTapesCell = (struct listOfTapes*)malloc(sizeof(*newListOfTapesCell));
        struct tapeCell *newTapeCell = (struct tapeCell*)malloc(sizeof(*newTapeCell));
        if (newListOfTapesCell == NULL || newTapeCell == NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        newTapeCell->data = voidSymbol;
        newTapeCell->next = NULL;
        newTapeCell->prev = NULL;
        newTapeCell->position = 1;
        newListOfTapesCell->thisTape = newTapeCell;
        newListOfTapesCell->curSymbol = newTapeCell->data;
        newListOfTapesCell->curPosition = newTapeCell->position;
        newListOfTapesCell->nextTape = NULL;

        //вставляем новую ленту в конец списка лент
        struct listOfTapes *curListOfTapes = *tapes;
        if (curListOfTapes == NULL){
            newListOfTapesCell->tapeNumb = 1;
            *tapes = newListOfTapesCell;
        }
        else{
            while (curListOfTapes->nextTape != NULL) curListOfTapes = curListOfTapes->nextTape;
            newListOfTapesCell->tapeNumb = (curListOfTapes->tapeNumb +1);
            curListOfTapes->nextTape = newListOfTapesCell;
        }
    }

    char inputString[2000000];
    while(fgets(inputString, sizeof(inputString), input)){
        char *tapeNum = strcut(inputString, 4, strlen(inputString) - 1);
        if (atoi(tapeNum) == 0) {
            printf("Error: wrong tape number: %i.\n", atoi(tapeNum));
            exit(10);
        }

        //находим нужную ленту
        struct listOfTapes *curListOfTapesCell = *tapes;
        while(curListOfTapesCell != NULL && curListOfTapesCell->tapeNumb != atoi(tapeNum)) curListOfTapesCell = curListOfTapesCell->nextTape;
        if (curListOfTapesCell == NULL){
            printf("Error: total tapes: %i, required tape: %i.\n", numOfTapes, atoi(tapeNum));
            exit(11);
        }
        free(tapeNum);

        //cчитываем положение каретки
        fgets(inputString, sizeof(inputString), input);
        if (inputString == NULL){
            printf("Error: file reading error: no enough strings in file %s", fileName);
            exit(12);
        }
        int carriagePosition = 0;
        while (inputString[carriagePosition] != 'v' && inputString[carriagePosition] != '\n' && inputString[carriagePosition] != '\0') carriagePosition++;
        if (inputString[carriagePosition] == '\n' || inputString[carriagePosition] == '\0') {
            printf("Eror: no pointer (v) in input file.\n");
            exit(13);
        }
        curListOfTapesCell->curPosition = carriagePosition +1;

        //считываем ленту
        fgets(inputString, sizeof(inputString),input);
        if (inputString == NULL){
            printf("Error: file reading error: no enough strings in file %s", fileName);
            exit(12);
        }
        char *tape = strdup(inputString);
        if (tape == NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        if(tape[strlen(tape)-1] == '\n') tape = strcut(tape,0,strlen(tape)-1); // избавляемся от символа переноса
        int tapeLength = strlen(tape);
        if (carriagePosition >= tapeLength){
            printf("Error: pointer (v) out of tape.\n");
            exit(14);
        }
        //записываем ленту
        tapeLength = strlen(tape);
        for (int i = 0; i < tapeLength; ++i) {
            //записываем символ в конец ленты
            struct tapeCell *curTapeCell = curListOfTapesCell->thisTape;
            while (curTapeCell->next != NULL) curTapeCell = curTapeCell->next;
            curTapeCell->data = tape[i];
            if (i == carriagePosition) curListOfTapesCell->curSymbol = inputString[carriagePosition];

            //добавляем пустой символ в конец ленты
            struct tapeCell *newTapeCell = (struct tapeCell*)malloc(sizeof(*newTapeCell));
            if (newTapeCell == NULL) {
                printf("Error: not enough memory.\n");
                exit(1);
            }
            newTapeCell->next = NULL;
            newTapeCell->data = voidSymbol;
            newTapeCell->prev = curTapeCell;
            newTapeCell->position = (curTapeCell->position +1);
            curTapeCell->next = newTapeCell;
        }
        free(tape);

    }

    fclose(input);
}