#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

int Stack_ctor(my_stack *stk, const char *name, int line, const char *file, const char *func);
void Stack_dtor(my_stack *stk);
int Stack_push(my_stack *stk, Elem_t value);
int Stack_pop(my_stack *stk, int *x);
int Fill_stack(my_stack *stk);
int Stack_realloc(my_stack *stk, For_realloc change_capacity);


#endif // STACK_H_INCLUDED
