#ifndef MINOR_FUNC_H_INCLUDED
#define MINOR_FUNC_H_INCLUDED

#include"stack.h"

enum Resize
{
    DO_NOTHING,
    CHANGE
};

const int INCREASE_CAPACITY = 2;
const int DECRISE_CAPACITY = 2;
const int POISON = 333;

int Fill_poison(my_stack *stk);
int Stack_realloc(my_stack *stk);
int Change_capacity_if_need(my_stack *stk, Resize *mode);

#endif // MINOR_FUNCT_H_INCLUDED
