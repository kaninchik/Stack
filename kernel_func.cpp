#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"stack.h"
#include"security.h"
#include"kernel_func.h"
#include"dump.h"


int Fill_poison(my_stack *stk)
{
    for(int i = stk->size_stack; i < stk->capacity; i++)
        stk->data[i] = POISON;

    return stk->status;
}

int Stack_realloc(my_stack *stk)
{
    Resize mode = DO_NOTHING;

    Change_capacity_if_need(stk, &mode);

    switch(mode)
    {
        case CHANGE:
        {

            #ifdef STK_PROTECT

            stk->data = (Elem_t *)((char *)stk->data - sizeof(Canary_t));
            stk->data = (Elem_t *)realloc(stk->data, stk->capacity*sizeof(Elem_t) + 2*sizeof(Canary_t));

            #else

            stk->data = (Elem_t *)realloc(stk->data, stk->capacity*sizeof(Elem_t));

            #endif

            break;
        }

        case DO_NOTHING:
            return stk->status;

        default:
            assert(!"Unknown command");
    }

    if(stk->data == NULL)
    {
        stk->status |= STK_DATA_IS_NULL;

        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    #ifdef STK_PROTECT

    stk->data = (Elem_t *)((char *)stk->data + sizeof(Canary_t));

    *(Canary_t *)((Elem_t *)(stk->data) + stk->capacity) = STK_CANARY;

    #endif

    return stk->status;
}

int Change_capacity_if_need(my_stack *stk, Resize *mode)
{
    if(mode == NULL)
    {
        stk->status = ENUM_POINTER_IS_NULL;

        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    if(stk->size_stack + 1 >= stk->capacity)
    {
        stk->capacity = INCREASE_CAPACITY*stk->capacity;

        *mode = CHANGE;
    }

    if(stk->size_stack < stk->capacity/2
            && stk->capacity > 4)
    {
        stk->capacity = stk->capacity/DECRISE_CAPACITY;

        *mode = CHANGE;
    }

    return stk->status;
}
