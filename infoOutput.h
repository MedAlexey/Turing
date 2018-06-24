#ifndef TURING_INFOOUTPUT_H
#define TURING_INFOOUTPUT_H

void printFinalIssues();
void printIssues();
void printCurState(int numOfStep, int curState, struct listOfTapes *tapes);
void printTapes(struct listOfTapes *tapes);
void foutCondition(char *file, int numOfStep,struct listOfTapes *tapes);

#endif //TURING_INFOOUTPUT_H
