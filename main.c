#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "infoInput.h"
#include "infoOutput.h"
#include "strcut.h"



extern void step(char *, int*, int*, struct listOfTapes*, struct listOfStates*);


int main(int argc, char *argv[]) {
    char *descriptionFile, *outputFile, *tapeFile;                 //files
    int curState, numOfTapes, numOfStep = 0, reqSteps = 0;
    char *modeFlag = NULL;
    char *alfabet = NULL;
    char voidSymbol;
    struct listOfTapes *tapes = NULL;
    struct listOfStates *states = NULL;

    states = (struct listOfStates *) malloc(sizeof(*states));
    if (states == NULL) {
        printf("Error: not enough memory.\n");
        exit(1);
    }
    states->stateNumb = 1;
    states->nextState = NULL;
    states->thisState = NULL;

    if (argc < 4 || argc >5) {
        printf("Error: wrong number on parameters");
        exit(15);
    }
    descriptionFile = argv[1];
    tapeFile = argv[2];
    outputFile = argv[3];
    modeFlag = argv[4];



    //читаем файл установок и ленту
    readDescription(descriptionFile, &alfabet, &voidSymbol, &numOfTapes, &curState, states);
    readTapes(tapeFile, voidSymbol, numOfTapes, &tapes);

    while(curState != 0){
        if (modeFlag != NULL) {
            if (strcmp(modeFlag, "-d") != 0) {
                printf("Error: wrong flag.\n");
                exit(2);
            }

            if (strcmp(modeFlag, "-d") == 0 && reqSteps == 0) {
                int choice;
                int err;
                do {
                    printIssues();
                    err = scanf("%d", &choice);
                    if (err != 1 || choice < 1 || choice > 6) {
                        printf("Wrong choise.\n");
                        choice = 0;
                        fflush(stdin);
                    }
                    switch (choice) {
                        case 1:
                            break;
                        case 2:
                            printf("Enter number of required steps.\n");
                             err = scanf("%d", &reqSteps);
                             if (err != 1) {
                                 printf("Wrong number of steps.\n");
                                 reqSteps = 0;
                                 fflush(stdin);
                             }
                            if (reqSteps != 0) choice = 1;
                            break;
                        case 3:
                            printTapes(tapes);
                            break;
                        case 4:
                            printCurState(numOfStep, curState, tapes);
                            break;
                        case 5:
                            foutCondition(outputFile, numOfStep, tapes);
                            printf("Done.\n");
                            break;
                        case 6:
                            modeFlag = NULL;
                            choice = 1;
                            break;
                        default:
                            break;
                    }
                } while (choice != 1);
            }
        }
        if (reqSteps > 0) reqSteps--;
        step(&voidSymbol, &numOfStep, &curState, tapes, states);
    }

    //вопрос перед завершением работы программы
    int choice = 0;
    while (choice != 1) {
        printFinalIssues();
        int err = scanf("%d", &choice);
        if (err != 1 || choice < 1 || choice > 4) {
            printf("Wrong choise.\n");
            fflush(stdin);
            choice = 0;
        }
        switch (choice){
            case 1:
                break;
            case 2:
                printTapes(tapes);
                break;
            case 3:
                printCurState(numOfStep, curState, tapes);
                break;
            case 4:
                foutCondition(outputFile, numOfStep, tapes);
                printf("Done.\n");
                break;
            default:
                break;
        }
    }

    return 0;
}


void step(char *voidSymbol, int * numOfStep, int *curState, struct listOfTapes *tapes, struct listOfStates *states) {
    *numOfStep = *numOfStep + 1;
    //находим нужное состояние
    struct listOfStates *curListOfStatesCell = states;
    while (curListOfStatesCell != NULL && curListOfStatesCell->stateNumb != *curState) curListOfStatesCell = curListOfStatesCell->nextState;
    if(curListOfStatesCell == NULL) {
        printf("Error: no suitable state on step num.%i.\n", *numOfStep);
        exit(3);
    }

    //ищем подходящую часть состояния
    struct state *curStateCell = curListOfStatesCell->thisState;
    while (curStateCell != NULL) {
        char *curCondition = strdup(curStateCell->curCondition);
        if (curCondition == NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        curCondition = strtok(curCondition, ",");
        struct listOfTapes *curListOfTapesCell = tapes;

        //сравнение condition с tapes
        while (curListOfTapesCell != NULL) {
            if (curCondition == NULL) {
                printf("Error: less curCondition on step num.%i", *numOfStep);
                exit(4);
            }
            if (curListOfTapesCell->curSymbol != *curCondition) break;

            curListOfTapesCell = curListOfTapesCell->nextTape;
            curCondition = strtok(NULL, ",");
        }

        if (curListOfTapesCell == NULL) break;

        curStateCell = curStateCell->next;
        free(curCondition);
    }

    if (curStateCell == NULL) {
        printf("Error: no suitable state condition no step num.%i.", *numOfStep);
        exit(5);
    }

    //переписываем символы под каретками и делаем шаги
    char *nextCondition = strdup(curStateCell->newCondition);
    if (nextCondition == NULL){
        printf("Error: not enough memory.\n");
        exit(1);
    }
    struct listOfTapes *curListOfTapesCell = tapes;
    while (curListOfTapesCell != NULL) {
        //доходим до положения каретки
        struct tapeCell *curTapeCell = curListOfTapesCell->thisTape;
        while (curListOfTapesCell->curPosition != curTapeCell->position) curTapeCell = curTapeCell->next;

        //записываем в ленту новый символ
        char *newData = strdup(nextCondition);
        if (newData ==  NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        newData = strtok(newData, ",");
        curTapeCell->data = *newData;
        nextCondition = strcut(nextCondition, 2, strlen(nextCondition));

        //делаем шаг
        char *stepDir = strdup(nextCondition);
        if (stepDir == NULL){
            printf("Error: not enough memory.\n");
            exit(1);
        }
        stepDir = strtok(stepDir, ";");
        nextCondition = strcut(nextCondition, 2, strlen(nextCondition));
        switch (*stepDir) {
            case 'L':
                if (curTapeCell->prev == NULL) {
                    struct tapeCell *newTapeCell = (struct tapeCell *) malloc(sizeof(*newTapeCell));
                    if (newTapeCell == NULL) {
                        printf("Error: not enough memory.\n");
                        exit(1);
                    }
                    newTapeCell->next = curTapeCell;
                    newTapeCell->data = *voidSymbol;
                    newTapeCell->prev = NULL;
                    newTapeCell->position = curTapeCell->position -1;
                    curTapeCell->prev = newTapeCell;
                    curListOfTapesCell->thisTape = newTapeCell;
                }
                curListOfTapesCell->curPosition = curTapeCell->prev->position;
                curListOfTapesCell->curSymbol = curTapeCell->prev->data;
                break;
            case 'R':
                if (curTapeCell->next == NULL) {
                    struct tapeCell *newTapeCell = (struct tapeCell *) malloc(sizeof(*newTapeCell));
                    if (newTapeCell == NULL) {
                        printf("Error: not enough memory.\n");
                        exit(1);
                    }
                    newTapeCell->next = NULL;
                    newTapeCell->data = *voidSymbol;
                    newTapeCell->prev = curTapeCell;
                    newTapeCell->position = curTapeCell->position + 1;
                    curTapeCell->next = newTapeCell;
                }
                curListOfTapesCell->curPosition = curTapeCell->next->position;
                curListOfTapesCell->curSymbol = curTapeCell->next->data;
                break;
            default:
                if (*stepDir != 'S') {
                    printf("Error: wrong step condition in step num.%i.", *numOfStep);
                    exit(6);
                }
                curListOfTapesCell->curSymbol = curTapeCell -> data;
                break;
        }

        curListOfTapesCell = curListOfTapesCell->nextTape;

        free(newData);
        free(stepDir);
    }

    *curState = curStateCell->nextState;
    free(nextCondition);
}