#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct Stack
{
    char **data;
    int size;
    int max_size;
};
typedef struct Stack Stack;


void stackInitialize(Stack *s, int _max_size)
{
    s->data = malloc(sizeof(char*)*_max_size);
    s->size = 0;
    s->max_size = _max_size;
}


void stackPush(Stack *s, char *string)
{
    if(s->size < s->max_size)
    {
        s->data[s->size] = malloc(sizeof(char)*strlen(string));
        strcpy(s->data[s->size], string);
		s->size++;
    }
    else
    {
		fprintf(stderr, "Error: Stack is full\n");
    }
}


void stackPop(Stack *s, char *new_string)
{
    if(s->size == 0)
    {
		fprintf(stderr, "Error: Stack is empty\n");
    }
    else
    {
		s->size--;
		strcpy(new_string, s->data[s->size]);
		free(s->data[s->size]);
    }
}
