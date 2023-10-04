#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#define STK_PROTECT

#ifdef STK_PROTECT

typedef uint64_t Canary_t;

const Canary_t STK_CANARY = 123456789;
const int INITIAL_CAPACITY = 3;

#endif

enum Errors
{
    NO_ERRORS = 0,
    STACK_IS_NULL = 1 << 0,
    STK_DATA_IS_NULL = 1 << 1,
    CAPACITY_LESS_THAN_ZERO = 1 << 2,
    SIZE_LESS_THAN_ZERO = 1 << 3,
    SIZE_BIGGER_THAN_CAPACITY = 1 << 4,
    STK_CANARY_DESTROYED = 1 << 5,
    ARR_CANARY_DESTROYED = 1 << 6,
    STK_HASH_DESTROYED = 1 << 7,
    VALUE_POINTER_IS_NULL = 1 << 8
};

typedef int Elem_t;

struct my_stack
{
    #ifdef STK_PROTECT

    Canary_t left_stk_canary = 0;
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

    #ifdef STK_PROTECT

    Canary_t right_stk_canary = 0;

    #endif
};

int Stack_ctor(my_stack *stk);
void Stack_dtor(my_stack *stk);
int Stack_push(my_stack *stk, Elem_t value);
int Stack_pop(my_stack *stk, Elem_t *x);

#endif // STACK_H_INCLUDED
