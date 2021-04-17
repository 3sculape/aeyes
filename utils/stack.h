#ifndef STACK_H
#define STACK_H

typedef struct stack
{
    void* data;
    char* detail;
    struct stack* next;
    //struct stack* prev;
}stack;

// Initializes an empty stack, programmer mallocs the stack and then init
void init_stack(stack* s);

/* Pop the first element and reorders the structure
 * Takes as parameter a pointer to the sentinel and returns the data stored
 * in the first element, after freeing it
 * Note: the popped element is freed, but the data is not.
 * The fate of the popped element is up to the programmer.
 */
void* pop_stack(stack* s);

/* Pushes an element at the head of the stack, takes as parameter the stack and
 * a pointer to the data that we want to push
 * We consider that the data is mallocd outside of the function
 */
void push_stack(stack* s, void* data);

// Simple print of each element for debugging purposes
void print_stack(stack* s);

size_t stack_len(stack* s);

// Frees every element in the stack to prevent the spicy memleaks
// Sentinel must be freed by the programmer
void clear_stack(stack* s);
void clear_stack_text(stack* s);

int stack_isempty(stack* s);

#endif
