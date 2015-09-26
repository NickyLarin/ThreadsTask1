#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct StackInt
{
    int *data;
    int size;
    int max_size;
};
typedef struct StackInt StackInt;


void stackIntInitialize(StackInt *s, int _max_size)
{
    s->data = malloc(sizeof(int)*_max_size);
    s->size = 0;
    s->max_size = _max_size;
}


void stackIntPush(StackInt *s, int i)
{
    if(s->size < s->max_size)
    {
        s->data[s->size] = i;
		s->size++;
    }
    else
    {
		fprintf(stderr, "Error: Stack is full\n");
    }
}


void stackIntPop(StackInt *s, int *new_i)
{
    if(s->size == 0)
    {
		fprintf(stderr, "Error: Stack is empty\n");
    }
    else
    {
		s->size--;
		*new_i = s->data[s->size];
    }
}

