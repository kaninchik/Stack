#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

#define STACK_CTOR(stk) Stack_ctor(stk, #stk, __LINE__, __FILE__, __func__)
#define STACK_DUMP(stk) Stack_dump(stk, #stk, __LINE__, __FILE__, __func__)

enum Errors {NO_ERRORS = 0, ERROR_WITH_ARRAY = 1, ERROR_WITH_STACK = 2, ERROR_WITH_CAPACITY = 4, ERROR_WITH_SIZE = 8, CANARY_STK_DEAD = 16, CANARY_ARR_DEAD = 32, ERROR_WITH_HASH = 64};
enum For_realloc {ADD, REDUCE};

typedef int Elem_t;

#ifdef DEBUG

typedef uint64_t Canary_t;

const Canary_t CANARY = 123456789;

#endif

const int POISON = 333;

struct my_stack
{
    #ifdef DEBUG

    Canary_t left_canary = 0;
    uint64_t hash = 0;

    #endif

    Elem_t *data = NULL;
    int size_stack = 0;
    int capacity = 0;
    int status = NO_ERRORS;
    const char *file = NULL;
    const char *name = NULL;
    const char *func = NULL;
    int line = 0;

    #ifdef DEBUG

    Canary_t right_canary = 0;

    #endif
};


#endif // CONSTS_H_INCLUDED
