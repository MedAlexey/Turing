#include <stdio.h>
#include "structures.h"
#include "infoOutput.h"


void printFinalIssues(){
    printf("The program is finished. Select one of the options:\n");
    printf("1) Exit.\n");
    printf("2) Display taps to the console.\n");
    printf("3) Display the current state of machine to the console (tape number, current symbol, current position).\n");
    printf("4) Output the current state of the machine to the file.\n");
}

void printIssues(){
    printf("Select one of the options:\n");
    printf("1) Make step.\n");
    printf("2) Make a certain number of steps.\n");
    printf("3) Display taps to the console.\n");
    printf("4) Display the current state of machine to the console (tape number, current symbol, current position).\n");
    printf("5) Output the current state of the machine to the file.\n");
    printf("6) Exit debug mode.\n");
}

void printCurState(int numOfStep, int curState, struct listOfTapes *tapes) {
    printf("----------------------\n");
    printf("Step num.%i\n", numOfStep);
    printf("Current state: q%i\n", curState);
    printf("Tapes:\n");

    struct listOfTapes *curListOfTapesCell = tapes;
    while (curListOfTapesCell != NULL) {
        printf("    Tape num.%i:\n\tsymbol: %c, position: %i.\n", curListOfTapesCell->tapeNumb, curListOfTapesCell->curSymbol, curListOfTapesCell->curPosition);
        curListOfTapesCell = curListOfTapesCell->nextTape;
    }

    printf("----------------------\n");
}

void printTapes(struct listOfTapes *tapes) {
    printf("----------------------\n");
    struct listOfTapes *curListOfTapesCell = tapes;
    while (curListOfTapesCell != NULL) {
        printf("Tape num. %i:\n\t", curListOfTapesCell->tapeNumb);

        //печатаем положение каретки
        struct tapeCell *curTapeCell = curListOfTapesCell->thisTape;
        while (curTapeCell != NULL) {
            if (curListOfTapesCell->curPosition == curTapeCell->position) printf("%c", 'v');
            else printf("%c", '.');

            curTapeCell = curTapeCell->next;
        }
        printf("\n\t");

        //печатаем ленты
        curTapeCell = curListOfTapesCell->thisTape;
        while (curTapeCell != NULL) {
            printf("%c",/* curTapeCell->position,*/ curTapeCell->data);

            curTapeCell = curTapeCell->next;
        }
        printf("\n");
        curListOfTapesCell = curListOfTapesCell->nextTape;
    }

    printf("----------------------\n");
}

void foutCondition(char *file, int numOfStep,struct listOfTapes *tapes) {
    FILE *output;
    output = fopen(file, "w");

    fprintf(output, "Number of steps: %i;\n", numOfStep);

    struct listOfTapes *curListOfTapesCell = tapes;
    while (curListOfTapesCell != NULL) {
        fprintf(output, "Tape number: %i;\ncurrent position: %i;\n\t", curListOfTapesCell->tapeNumb, curListOfTapesCell->curPosition);
        //выводим положение каретки
        struct tapeCell *curTapeCell = curListOfTapesCell->thisTape;
        while (curTapeCell != NULL){
            if (curListOfTapesCell->curPosition == curTapeCell->position) fprintf(output,"%c", 'v');
            else fprintf(output, "%c", '.');

            curTapeCell = curTapeCell->next;
        }
        fprintf(output, "\n\t");
        //выводим ленту
        curTapeCell = curListOfTapesCell->thisTape;
        while (curTapeCell != NULL) {
            fprintf(output, "%c", curTapeCell->data);
            curTapeCell = curTapeCell->next;
        }
        fputs("\n\n", output);
        curListOfTapesCell = curListOfTapesCell->nextTape;
    }

    fclose(output);
}
