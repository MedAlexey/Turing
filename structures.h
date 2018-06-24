#ifndef TURING_STRUCTURES_H
#define TURING_STRUCTURES_H
//list_of_tapes
struct listOfTapes
{
    int                 tapeNumb;
    char                curSymbol;
    int                 curPosition;
    struct tapeCell*    thisTape;
    struct listOfTapes* nextTape;
};

//tape
struct tapeCell
{
    int              position;
    char             data;
    struct tapeCell* next;
    struct tapeCell* prev;
};

//list_of_states
struct listOfStates
{
    int                  stateNumb;
    struct state*        thisState;
    struct listOfStates* nextState;
};

//state
struct state {
    char*         curCondition;
    char*         newCondition;
    int           nextState;
    struct state* next;
};
#endif //TURING_STRUCTURES_H
