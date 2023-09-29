#ifndef SECURITY_H_INCLUDED
#define SECURITY_H_INCLUDED

int Stack_Ok(my_stack *stk);
int Stack_dump(my_stack *stk, const char *name, int line, const char *file, const char *func);
int Stack_hash(my_stack *stk);
int Check_hash(my_stack *stk);

#endif // SECURITY_H_INCLUDED
