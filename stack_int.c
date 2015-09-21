#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_MAX 100

struct StackInt {
    int data[STACK_MAX];
    int size;
};
typedef struct StackInt StackInt;

void stackIntInit(StackInt *S)
{
    S->size = 0;
}

int stackIntPop(StackInt *S)
{
    if (S->size == 0)
    {
        fprintf(stderr, "Error: stack empty\n");
        return -1;
    }
    S->size--;
    return S->data[S->size];
}

void stackIntPush(StackInt *S, int d)
{
    if (S->size < STACK_MAX)
    {
        S->data[S->size++] = d;
    }
    else
    {
        fprintf(stderr, "Error: stack full\n");
    }
}