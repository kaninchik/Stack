#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<cstdint>

#define DEBUG
#define STACK_CTOR(stk) Stack_ctor(stk, #stk, __LINE__, __FILE__, __func__)
#define STACK_DUMP(stk) Stack_dump(stk, #stk, __LINE__, __FILE__, __func__)

enum Errors {NO_ERRORS = 0, ERROR_WITH_ARRAY = 1, ERROR_WITH_STACK = 2, ERROR_WITH_CAPACITY = 4, ERROR_WITH_SIZE = 8, CANARY_STK_DEAD = 16, CANARY_ARR_DEAD = 32, ERROR_WITH_HASH = 64};
enum For_realloc {ADD, REDUCE};

typedef int Elem_t;
typedef uint64_t Canary_t;

const int POISON = 333;
const Canary_t CANARY = 123456789;

struct my_stack
{
    #ifdef DEBUG

    Canary_t left_canary = 0;

    #endif

    Elem_t *data = NULL;
    int size_stack = 0;
    int capacity = 0;

    #ifdef DEBUG

    int status = NO_ERRORS;
    const char *file = NULL;
    const char *name = NULL;
    const char *func = NULL;
    int line = 0;
    Canary_t right_canary = 0;
    uint64_t hash = 0;


    #endif
};

int Stack_ctor(my_stack *stk, const char *name, int line, const char *file, const char *func);
void Stack_dtor(my_stack *stk);
int Stack_push(my_stack *stk, Elem_t value);
int Stack_pop(my_stack *stk, int *x);
int Stack_dump(my_stack *stk, const char *name, int line, const char *file, const char *func);
int Fill_stack(my_stack *stk);
int Stack_Ok(my_stack *stk);
int Stack_realloc(my_stack *stk, For_realloc change_capacity);
int Stack_hash(my_stack *stk);
int Check_hash(my_stack *stk);



int main()
{
    my_stack stk;

    STACK_CTOR( &stk);

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


int Stack_ctor(my_stack *stk, const char *name, int line, const char *file, const char *func)
{
    stk->left_canary = CANARY;
    stk->size_stack = 0;
    stk->capacity = 3;

    #ifdef DEBUG

    stk->name = name;
    stk->line = line;
    stk->file = file;
    stk->func = func;

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

    #endif

    Fill_stack(stk);

    Stack_hash(stk);

    Stack_Ok(stk);

    STACK_DUMP(stk);

    return stk->status |= NO_ERRORS;

}

void Stack_dtor(my_stack *stk)
{
    free((char *)stk->data - sizeof(Canary_t));
    free(stk);

    stk->data = NULL;
    stk->size_stack = -1;
    stk->capacity = 0;
    stk->file = NULL;
    stk->name = NULL;
    stk->func = NULL;
    stk->line = 0;
}

int Stack_push(my_stack *stk, Elem_t value)
{
    if(!Check_hash(stk))
    {
        stk->status |= ERROR_WITH_HASH;
        STACK_DUMP(stk);
    }

    if(stk->size_stack + 1 >= stk->capacity)
        Stack_realloc(stk, ADD);

    stk->data[stk->size_stack] = value;

    stk->size_stack++;

    Fill_stack(stk);

    Stack_hash(stk);

    Stack_Ok(stk);

    STACK_DUMP(stk);

    return stk->status |= NO_ERRORS;

}

int Stack_pop(my_stack *stk, int *x)
{
    if(!Check_hash(stk))
    {
        stk->status |= ERROR_WITH_HASH;
        STACK_DUMP(stk);
    }

    *x = *(stk->data + stk->size_stack - 1);

    if(stk->size_stack - 1 < stk->capacity/2 && stk->capacity > 4)
        Stack_realloc(stk, REDUCE);

    stk->size_stack--;

    Fill_stack(stk);

    Stack_hash(stk);

    Stack_Ok(stk);

    STACK_DUMP(stk);

    return stk->status |= NO_ERRORS;
}

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

        printf("%p [%d] = %lu\n", (Canary_t *)(stk->data) - 1, -1, *((Canary_t *)(stk->data) - 1));

        for(int i = 0; i <= stk->capacity; i++)
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

                    case CANARY_STK_DEAD:

                        printf("The canaries guarding the stack were killed\n");
                        break;

                    case CANARY_ARR_DEAD:

                        printf("The canaries guarding the array were killed\n");
                        break;

                    case ERROR_WITH_HASH:
                        printf("Hash doesn't match");
                        break;
                }
            }
        }
    }

    if(stk->status == 0)
    {
        printf("size = %d;\n"
               "capacity = %d;\n"
               "data[%p]:\n", stk->size_stack, stk->capacity, stk->data);

         for(int i = 0; i < stk->size_stack; i++)
            printf("%p [%d] = %d\n", (stk->data + i), i, *(stk->data + i));
    }

    return stk->status = NO_ERRORS;
}

int Fill_stack(my_stack *stk)
{
    for(int i = stk->size_stack; i < stk->capacity; i++)
        *(stk->data + i) = POISON;

    return stk->status |= NO_ERRORS;

    Stack_Ok(stk);

    STACK_DUMP(stk);
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

    if(stk->left_canary != CANARY || stk->right_canary != CANARY)
        stk->status |= CANARY_STK_DEAD;

    if(((Elem_t *)(stk->data))[stk->capacity] != CANARY || ((Canary_t *)(stk->data))[-1] != CANARY)
        stk->status |= CANARY_ARR_DEAD;

    if(!Check_hash(stk))
        stk->status |= ERROR_WITH_HASH;

    return stk->status;

}

int Stack_realloc(my_stack *stk, For_realloc change_capacity)
{

    Canary_t tmp = ((Elem_t *)(stk->data))[stk->capacity];

    if(change_capacity == REDUCE)
    {
        stk->data = (Elem_t *)realloc((char *)stk->data - sizeof(Canary_t), stk->capacity/2 * sizeof(Elem_t) + 2*sizeof(Canary_t));

        if(stk->data == NULL)
        {
            return stk->status |= ERROR_WITH_ARRAY;

            STACK_DUMP(stk);
        }

        stk->capacity = stk->capacity/2;
    }

    if(change_capacity == ADD)
    {
        stk->data = (Elem_t *)realloc((char *)stk->data - sizeof(Canary_t), 2*stk->capacity*sizeof(Elem_t) + 2*sizeof(Canary_t));

        if(stk->data == NULL)
        {
            return stk->status |= ERROR_WITH_ARRAY;

            STACK_DUMP(stk);
        }

        stk->capacity = 2*stk->capacity;

    }

    stk->data = (Elem_t *)((char *)stk->data + sizeof(Canary_t));

    ((Elem_t *)(stk->data))[stk->capacity] = tmp;

    return stk->status |= NO_ERRORS;
}

int Stack_hash(my_stack *stk)
{
    stk->hash = 5381;

    for(size_t i = 0; i < sizeof(my_stack); i++)
        stk->hash = ((stk->hash << 5) + stk->hash) + *((char *)stk + i);

    for(int i = -1; i <= stk->capacity; i++)
        stk->hash = ((stk->hash << 5) + stk->hash) + *((char *)stk->data + i);

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




