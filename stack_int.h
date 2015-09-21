#ifndef STACK_INT_H
#define STACK_MAX 100
struct StackInt
{
    int data[STACK_MAX];
    int size;
};
typedef struct StackInt StackInt;
void stackIntInit(StackInt *S);
int stackIntPop(StackInt *S);
void stackIntPush(StackInt *S, int d);
#define STACK_INT_H
#endif