#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_MAX 100

struct Stack {
    char * data[STACK_MAX];
    int size;
};
typedef struct Stack Stack;

void stackInit(Stack *S)
{
    S->size = 0;
}

char * stackPop(Stack *S)
{
    if (S->size == 0)
    {
        fprintf(stderr, "Error: stack empty\n");
        return (char *)-1;
    }
    S->size--;
    //char temp[100];
    //strcpy(temp, S->data[S->size]);
    //printf("%s", temp);
    //free(S->data[S->size]);
    return S->data[S->size];
}

void stackPush(Stack *S, char *d)
{
    if (S->size < STACK_MAX)
    {
        char *temp = malloc(sizeof(char)*strlen(d));
        strcpy(temp, d);
        S->data[S->size++] = temp;
    }
    else
    {
        fprintf(stderr, "Error: stack full\n");
    }
}
