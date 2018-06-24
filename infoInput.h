#ifndef TURING_INFOREADING_H
#define TURING_INFOREADING_H

//void readDescription(char *, char **, char *, int *, int*, struct listOfStates *);
void readDescription(char *file, char ** alfabet, char *voidSymbol, int *numOfTapes, int* curState, struct listOfStates *states);
void splitState(char state[], struct listOfStates *states);
void readTapes(char *fileName, char voidSymbol, int numOfTapes, struct listOfTapes **tapes);


#endif //TURING_INFOREADING_H
