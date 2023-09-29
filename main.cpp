#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"consts.h"
#include"stack.h"
#include"security.h"

int main()
{
    my_stack stk;

    STACK_CTOR(&stk);

    int a = 0;

    Stack_push(&stk, 10);

    Stack_push(&stk, 10);

    Stack_push(&stk, 10);

    Stack_push(&stk, 10);

    Stack_pop(&stk, &a);

    Stack_pop(&stk, &a);

    Stack_dtor(&stk);

    return 0;

}





