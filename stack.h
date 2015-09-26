#ifndef STACK_H
	struct Stack
	{
		char **data;
		int size;
		int max_size;
	};
	typedef struct Stack Stack;
	void stackInitialize(Stack *s, int _max_size);
	void stackPush(Stack *s, char *string);
	void stackPop(Stack *s, char *new_string);
	#define STACK_H
#endif
