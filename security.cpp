#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#include"stack.h"
#include"security.h"
#include"kernel_func.h"
#include"dump.h"


int Stack_check(my_stack *stk)
{
    if(stk == NULL)
    {
        stk->status = STACK_IS_NULL;
        return stk->status;
    }

    if(stk->data == NULL)
    {
        stk->status = STK_DATA_IS_NULL;
        return stk->status;
    }

    if(stk->capacity < 0)
        stk->status |= CAPACITY_LESS_THAN_ZERO;

    if(stk->size_stack < 0)
        stk->status |= SIZE_LESS_THAN_ZERO;

    if(stk->capacity < stk->size_stack)
        stk->status |= SIZE_BIGGER_THAN_CAPACITY;

    #ifdef STK_PROTECT

    if(stk->left_stk_canary != STK_CANARY || stk->left_stk_canary != STK_CANARY)
        stk->status |= STK_CANARY_DESTROYED;

    if(*(Canary_t *)((Elem_t *)(stk->data) + stk->capacity) != STK_CANARY
        || ((Canary_t *)(stk->data))[-1] != STK_CANARY)
    {
        stk->status |= STK_DATA_CANARY_DESTROYED;
    }

    if(!Check_hash(stk))
        stk->status |= STK_HASH_DESTROYED;

    #endif

    return stk->status;

}

#ifdef STK_PROTECT
int Stack_hash(my_stack *stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    stk->hash = 5381;

    for(size_t i = 0; i < sizeof(my_stack); i++)
        stk->hash = ((stk->hash >> 5) + stk->hash) + *((char *)stk + i);

    for(int i = -sizeof(Canary_t); i <= stk->capacity; i++)
        stk->hash = ((stk->hash >> 5) + stk->hash) + *((char *)stk->data + i);

    return stk->status;

}


bool Check_hash(my_stack *stk)
{
    assert(stk != NULL);

    uint64_t prev_hash = stk->hash;

    stk->hash = 0;

    Stack_hash(stk);

    return prev_hash == stk->hash;
}
#endif
