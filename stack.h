#ifndef STACK_H
	#define STACK_MAX 100
	struct Stack
	{
		char * data[STACK_MAX];
		int size;
	};
	typedef struct Stack Stack;
	void stackInit(Stack *S);
	char * stackPop(Stack *S);
	void stackPush(Stack *S, char *d);
	#define STACK_H
#endif
