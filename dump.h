#ifndef DUMP_H_INCLUDED
#define DUMP_H_INCLUDED

#include"stack.h"

#define SPEC_ELEM_T "%d"
#define SPEC_CANARY_T "%llu"
#define STACK_DUMP_IF_ERROR(stk) Stack_dump(stk, #stk, __LINE__, __FILE__, __func__)

int Stack_dump(my_stack *stk, const char *name, int line, const char *file, const char *func);
int Print_stack(my_stack *stk);

#endif // DUMP_H_INCLUDED
