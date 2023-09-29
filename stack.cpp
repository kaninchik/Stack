#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"consts.h"
#include"stack.h"
#include"security.h"


int Stack_ctor(my_stack *stk, const char *name, int line, const char *file, const char *func)
{
    stk->size_stack = 0;
    stk->capacity = 3;
    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    #ifdef DEBUG

    stk->left_canary = CANARY;


    stk->right_canary = CANARY;

    stk->data = (Elem_t* )((char *)calloc(stk->capacity*sizeof(Elem_t) + 2*sizeof(Canary_t), 1));

    if(stk->data == NULL)
    {
        return stk->status |= ERROR_WITH_ARRAY;

        STACK_DUMP(stk);
    }

    stk->data = (Elem_t *)((char *)stk->data + sizeof(Canary_t));

    ((Canary_t *)(stk->data))[-1] = CANARY;

    ((Elem_t *)(stk->data))[stk->capacity] = CANARY;

    #else

    stk->data = (Elem_t* )calloc(stk->capacity, sizeof(Elem_t));

    if(stk->data == NULL)
    {
        return stk->status |= ERROR_WITH_ARRAY;

        STACK_DUMP(stk);
    }

    #endif

    Fill_stack(stk);

    #ifdef DEBUG

    Stack_hash(stk);

    #endif

    Stack_Ok(stk);

    STACK_DUMP(stk);

    return stk->status |= NO_ERRORS;

}

void Stack_dtor(my_stack *stk)
{
    stk->data = NULL;
    stk->size_stack = -1;
    stk->capacity = 0;
    stk->file = NULL;
    stk->name = NULL;
    stk->func = NULL;
    stk->line = 0;

    #ifdef DEBUG

    free((char *)stk->data - sizeof(Canary_t));

    #else

    free(stk->data);

    #endif

    free(stk);
}

int Stack_push(my_stack *stk, Elem_t value)
{
    #ifdef DEBUG

    if(!Check_hash(stk))
    {
        stk->status |= ERROR_WITH_HASH;
        STACK_DUMP(stk);
    }

    #endif

    if(stk->size_stack + 1 >= stk->capacity)
        Stack_realloc(stk, ADD);

    stk->data[stk->size_stack] = value;

    stk->size_stack++;

    Fill_stack(stk);

    #ifdef DEBUG

    Stack_hash(stk);

    #endif

    Stack_Ok(stk);

    STACK_DUMP(stk);

    return stk->status |= NO_ERRORS;

}

int Stack_pop(my_stack *stk, int *x)
{
    #ifdef DEBUG

    if(!Check_hash(stk))
    {
        stk->status |= ERROR_WITH_HASH;
        STACK_DUMP(stk);
    }

    #endif

    *x = *(stk->data + stk->size_stack - 1);

    if(stk->size_stack - 1 < stk->capacity/2 && stk->capacity > 4)
        Stack_realloc(stk, REDUCE);

    stk->size_stack--;

    Fill_stack(stk);

    #ifdef DEBUG

    Stack_hash(stk);

    #endif

    Stack_Ok(stk);

    STACK_DUMP(stk);

    return stk->status |= NO_ERRORS;
}

int Fill_stack(my_stack *stk)
{
    for(int i = stk->size_stack; i < stk->capacity; i++)
        *(stk->data + i) = POISON;

    return stk->status |= NO_ERRORS;

    Stack_Ok(stk);

    STACK_DUMP(stk);
}

int Stack_realloc(my_stack *stk, For_realloc change_capacity)
{
    #ifdef DEBUG

    Canary_t tmp = ((Elem_t *)(stk->data))[stk->capacity];

    #endif

    if(change_capacity == REDUCE)
    {
        #ifdef DEBUG

        stk->data = (Elem_t *)realloc((char *)stk->data - sizeof(Canary_t), stk->capacity/2 * sizeof(Elem_t) + 2*sizeof(Canary_t));

        #else

        stk->data = (Elem_t *)realloc(stk->data, stk->capacity/2 * sizeof(Elem_t));

        #endif

        if(stk->data == NULL)
        {
            return stk->status |= ERROR_WITH_ARRAY;

            STACK_DUMP(stk);
        }

        stk->capacity = stk->capacity/2;
    }

    if(change_capacity == ADD)
    {
        #ifdef DEBUG

        stk->data = (Elem_t *)realloc((char *)stk->data - sizeof(Canary_t), 2*stk->capacity*sizeof(Elem_t) + 2*sizeof(Canary_t));

        #else

        stk->data = (Elem_t *)realloc(stk->data, 2*stk->capacity*sizeof(Elem_t));

        #endif

        if(stk->data == NULL)
        {
            return stk->status |= ERROR_WITH_ARRAY;

            STACK_DUMP(stk);
        }

        stk->capacity = 2*stk->capacity;

    }

    #ifdef DEBUG

    stk->data = (Elem_t *)((char *)stk->data + sizeof(Canary_t));

    ((Elem_t *)(stk->data))[stk->capacity] = tmp;

    #endif

    return stk->status |= NO_ERRORS;
}

