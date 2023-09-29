#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"consts.h"
#include"stack.h"
#include"security.h"


int Stack_dump(my_stack *stk, const char *name, int line, const char *file, const char *func)
{
    if(stk->status != 0)
    {
        stk->name = name;
        stk->line = line;
        stk->file = file;
        stk->func = func;

        puts(stk->file);

        printf("function:\n");

        puts(stk->func);

        printf("line = %d;\n"
               "size = %d;\n"
               "capacity = %d;\n"
               "data[%p]:\n"
               "code of error = %d\n", (stk->line), stk->size_stack, stk->capacity, stk->data, stk->status);

        #ifdef DEBUG

        printf("%p [%d] = %lu\n", (Canary_t *)(stk->data) - 1, -1, *((Canary_t *)(stk->data) - 1));

        #endif

        for(int i = 0; i < stk->capacity; i++)
            printf("%p [%d] = %d\n", (stk->data + i), i, *(stk->data + i));

        for(int i = 1; i <= 128; i = i << 1)
        {
            if((stk->status & i) != 0)
            {
                switch(stk->status & i)
                {
                    case ERROR_WITH_ARRAY:

                        printf("The pointer to the array is NULL\n");
                        Stack_dtor(stk);
                        break;

                    case ERROR_WITH_STACK:

                        printf("The pointer to the stack is NULL\n");
                        Stack_dtor(stk);
                        break;

                    case ERROR_WITH_CAPACITY:

                        printf("The capacity of your stack is a negative number: %d\n", stk->capacity);
                        break;

                    case ERROR_WITH_SIZE:

                        printf("You are on a non_existent element: %d\n", stk->size_stack);
                        break;

                    #ifdef DEBUG

                    case CANARY_STK_DEAD:

                        printf("The canaries guarding the stack were killed\n");
                        break;

                    case CANARY_ARR_DEAD:

                        printf("The canaries guarding the array were killed\n");
                        break;

                    case ERROR_WITH_HASH:
                        printf("Hash doesn't match");
                        break;

                    #endif
                }
            }
        }

        stk->status = NO_ERRORS;
    }


    if(stk->status == 0)
    {
        for(int i = 0; i < stk->size_stack; i++)
            printf("%p [%d] = %d\n", (stk->data + i), i, *(stk->data + i));

        printf("\n");
    }


    return stk->status = NO_ERRORS;
}


int Stack_Ok(my_stack *stk)
{

    if(stk == NULL)
        return stk->status |= ERROR_WITH_STACK;

    if(stk->data == NULL)
        return stk->status |= ERROR_WITH_ARRAY;

    if(stk->capacity < 0)
        stk->status |= ERROR_WITH_CAPACITY;

    if(stk->size_stack < 0 || stk->capacity < stk->size_stack)
        stk->status |= ERROR_WITH_SIZE;

    #ifdef DEBUG

    if(stk->left_canary != CANARY || stk->right_canary != CANARY)
        stk->status |= CANARY_STK_DEAD;

    if(((Elem_t *)(stk->data))[stk->capacity] != CANARY || ((Canary_t *)(stk->data))[-1] != CANARY)
        stk->status |= CANARY_ARR_DEAD;

    if(!Check_hash(stk))
        stk->status |= ERROR_WITH_HASH;

    #endif

    return stk->status;

}


#ifdef DEBUG
int Stack_hash(my_stack *stk)
{
    stk->hash = 5381;

    for(size_t i = 0; i < sizeof(my_stack); i++)
        stk->hash = ((stk->hash >> 5) + stk->hash) + *((char *)stk + i);

    for(int i = -1; i <= stk->capacity; i++)
        stk->hash = ((stk->hash >> 5) + stk->hash) + *((char *)stk->data + i);

    return stk->status |= NO_ERRORS;

}


int Check_hash(my_stack *stk)
{
    uint64_t tmp = stk->hash;

    stk->hash = 0;

    Stack_hash(stk);

    if(tmp == stk->hash)
        return 1;
    else
        return 0;
}

#endif
