#ifndef STACK_H
	#define STACK_STRING 1
	#define STACK_INT 0
	struct Stack
	{
		void *data;
		int size;
		int max_size;
		int type;
	};
	typedef struct Stack Stack;
	void stackInitialize(Stack *s, int _max_size, int _type);
	void stackPush(Stack *s, void *value);
	void stackPop(Stack *s, void *new_value);
	void stackDestroy(Stack *s);
	#define STACK_H
#endif
