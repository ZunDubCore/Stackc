#include "stackc.h"

Stack *stackInit(int size)
{
	Stack *dest;
	stackElement *newStack;

	/* Allocate a new array to hold the contents. */
	dest = malloc(sizeof(stackElement*) + sizeof(int) * 2);
	newStack = malloc(sizeof(stackElement) * size);

	if (newStack == NULL)
	{
		error("Insufficient memory to initialize stack.\n");
	}

	dest->stack = newStack;
	dest->size = size;
	dest->top = EMPTY;  /* I.e., empty */

	return dest;
}

void stackDestroy(Stack *stack)
{
	/* Get rid of array. */
	free(stack->stack);
	free(stack);

	stack->stack = NULL;
	stack->size = 0;
	stack->top = EMPTY;  /* I.e., empty */
}

void stackPush(Stack *stack, stackElement element)
{
	if (stackIsFull(stack))
	{
		error("Can't push element on stack: stack is full.\n");
	}

	/* Put information in array; update top. */
	stack->stack[++stack->top] = element;
}

stackElement stackPop(Stack *stack)
{
	if (stackIsEmpty(stack))
	{
		error("Can't pop element from stack: stack is empty.\n");
	}

	return stack->stack[stack->top--];
}

BOOL stackIsEmpty(Stack *stack)
{
	return stack->top < 0;
}

BOOL stackIsFull(Stack *stack)
{
	return stack->top >= stack->size - 1;
}
