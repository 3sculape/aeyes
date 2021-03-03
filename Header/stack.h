#ifndef STACK_H
#define STACK_H

typedef struct stack
{
    void* data;
    char* detail;
    struct stack* next;
    //struct stack* prev;
}stack;

// Initializes an empty stack i.e returns a sentinel
stack* init_stack();

/* Pop the first element and reorders the structure
 * Takes as parameter a pointer to the sentinel and returns the data stored
 * in the first element, after freeing it
 * Note: the popped element is freed, but the data is not.
 * The fate of the popped element is up to the programmer.
 */
void* pop_stack(stack* s);

/* Pushes an element at the head of the stack, takes as parameter the stack and
 * a pointer to the data that we want to push
 * We consider that the data is mallocd outside of the function but the stack
 * structure is taken care of inside this function
 */
void push_stack(stack* s, void* data, char* detail);

// Simple print of each element for debugging purposes
void print_stack(stack* s);

// Frees every element in the stack to prevent the spicy memleaks
void clear_stack(stack* s);

#endif
