#include <stdio.h>
#include <stdlib.h>
#include "../Header/stack.h"
#include <err.h>

stack* init_stack()
{
    stack* sentinel = malloc(sizeof(stack*));
    sentinel->data = NULL;
    sentinel->next = NULL;
    return sentinel;
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

void push_stack(stack* s, void* data, char* detail)
{
    if (s == NULL)
        errx(EXIT_FAILURE, "Push : stack sentinel is NULL");
    if (data == NULL)
        warnx("Push : data is NULL");
    stack* new_el = malloc(sizeof(stack*));
    new_el->data = data;
    new_el->detail = detail;
    stack* rest_of_list = s->next;
    new_el->next = rest_of_list;
    s->next = new_el;
}

void print_stack(stack* s);
{
    size_t nb = 0;
    stack* start = s->next;
    for(; start != NULL; start = start->next)
    {
        printf("Element n°%lu : data pointer %p\n", nb, start->data);
        if (start->detail != NULL)
            printf("Details : %s\n\n", start->detail);
    }
}
