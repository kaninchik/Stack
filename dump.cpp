#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"stack.h"
#include"security.h"
#include"kernel_func.h"
#include"dump.h"

int Stack_dump(my_stack *stk, const char *name, int line, const char *file, const char *func)
{

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

    puts(stk->file);

    puts("function: ");

    puts(stk->func);

    printf("stack[%p]\n", stk);
    printf("line = %d\n", stk->line);
    printf("size = %d\n", stk->size_stack);
    printf("capacity = %d\n", stk->capacity);
    printf("date[%p]\n", stk->data);

    if(stk->data == NULL)
        return stk->status;

    #ifdef STK_PROTECT

    printf("CANARY ONE: %p [%d] = " SPEC_CANARY_T "\n", ((Canary_t *)(stk->data) - 1), -1, ((Canary_t *)(stk->data))[-1]);

    printf("CANARY TWO: %p [%d] = " SPEC_CANARY_T "\n", (Canary_t *)((Elem_t *)(stk->data) + stk->capacity),
            stk->capacity, *(Canary_t *)((Elem_t *)(stk->data) + stk->capacity));

    #endif

    for(int i = 0; i < stk->capacity; i++)
        printf("%p [%d] = " SPEC_CANARY_T "\n", (stk->data + i), i, stk->data[i]);

    for(int i = 1; i <= 256; i = i << 1)
    {
        switch(stk->status & i)
        {
            case NO_ERRORS:

                break;

            case STK_DATA_IS_NULL:

                printf("The pointer to the array is NULL\n");
                break;

            case STACK_IS_NULL:

                printf("The pointer to the stack is NULL\n");
                break;

            case CAPACITY_LESS_THAN_ZERO:

                printf("The capacity of your stack is a negative number: %d\n", stk->capacity);
                break;

            case SIZE_LESS_THAN_ZERO:

                printf("You are on a non_existent element: %d\n", stk->size_stack);
                break;

            case SIZE_BIGGER_THAN_CAPACITY:

                printf("You have gone off the stack");
                break;

            #ifdef PROTECT

            case STK_CANARY_DESTROYED:

                printf("The canaries guarding the stack were killed\n");
                break;

            case ARR_CANARY_DESTROYED:

                printf("The canaries guarding the array were killed\n");
                break;

            case STK_HASH_DESTROYED:

                printf("Hash doesn't match\n");
                break;

            #endif

            default:
                assert(!"It's unknown error");
        }
    }

    return stk->status;
}

int Print_stack(my_stack *stk)
{
    if(Stack_check(stk) != NO_ERRORS)
    {
        STACK_DUMP_IF_ERROR(stk);

        return stk->status;
    }

    for(int i = 0; i < stk->size_stack; i++)
        printf("element [%d] = " SPEC_ELEM_T "\n", i, stk->data[i]);

    printf("\n");

    return stk->status;
}
