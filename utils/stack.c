#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <err.h>
#include <SDL2/SDL.h>

void init_stack(stack* s)
{
    if (s == NULL)
        errx(EXIT_FAILURE, "STACK IS NULL");
    s->data = NULL;
    s->next = NULL;
    s->detail = NULL;
}

void* pop_stack(stack* s)
{
    if (s == NULL)
        errx(EXIT_FAILURE, "Pop : stack sentinel is NULL");
    stack* res = s->next;
    s->next = res->next;
    void* data = res->data;
    free(res);
    return data;
}

void push_stack(stack* s, void* data)
{
    if (s == NULL)
        errx(EXIT_FAILURE, "Push : stack sentinel is NULL");
    stack* new_el = malloc(sizeof(stack));
    new_el->next = NULL;
    new_el->data = data;
    stack* rest_of_list = s->next;
    new_el->next = rest_of_list;
    s->next = new_el;
}

void print_stack(stack* s)
{
    size_t nb = 0;
    stack* start = s->next;
    if (start == NULL)
        printf("Stack is empty !\n");
    for(; start != NULL; start = start->next)
    {
        printf("Element n°%lu : data pointer %p\n", nb, start->data);
        if (start->detail != NULL)
            printf("Details : %s\n\n", start->detail);
        nb ++;
    }
}

size_t stack_len(stack* s)
{
    if (stack_isempty(s))
        return 0;
    size_t count = 0;
    stack* start = s->next;
    for(; start != NULL; start = start->next)
    {
        count ++;
    }
    return count;
}

void clear_stack(stack* s)
{
    stack* head = s->next;
    while(head != NULL)
    {
        stack* tmp = head;
        head = head->next;
        free(tmp->data);
        free(tmp);
    }
    s->next = NULL;
}

void clear_stack_text(stack* s)
{
    stack* head = s->next;
    while(head != NULL)
    {
        stack* tmp = head;
        head = head->next;
        SDL_DestroyTexture(tmp->data);
        free(tmp);
    }
    s->next = NULL;
}

int stack_isempty(stack* s)
{
    if(s == NULL)
        printf("YESYY\n");
    return s->next == NULL;
}
