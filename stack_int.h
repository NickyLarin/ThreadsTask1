#ifndef STACK_INT_H
	struct StackInt
	{
		int *data;
		int size;
		int max_size;
	};
	typedef struct StackInt StackInt;
	void stackIntInitialize(StackInt *s, int _max_size);
	void stackIntPush(StackInt *s, int i);
	void stackIntPop(StackInt *s, int *new_i);
	#define STACK_INT_H
#endif
