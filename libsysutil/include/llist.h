#ifndef LLIST_H
#define LLIST_H

#include <stddef.h>

typedef struct LLNode {
    void *data;
    struct LLNode *next;
} LLNode;

<<<<<<< HEAD
typedef struct 
{
    LLNode *head;
    LLNode *tail;
    size_t length;
    void (*free_data)(void*);  // callback de liberation peut etre NULL

}LList;

LList *ll_create(void(*free_data)(void*));
int ll_push_back(LList *l, void* data);
int ll_push_front(LList *l,void* data);
void *ll_pop(LList *l);
void *ll_get(const LList *l, size_t index);
int ll_remove(LList *l, void *data, int)
=======
typedef struct {
    LLNode *head;
    LLNode *tail;
    size_t length;
    void (*free_data)(void *);
} LList;

/* callback de libération (peut être NULL) */
LList *ll_create(void (*free_data)(void *));
>>>>>>> 47c8fcf991530f15da7192efd4144b66c0e7ff9f

int ll_push_back(LList *l, void *data);

int ll_push_front(LList *l, void *data);

void *ll_pop_front(LList *l);

void *ll_get(const LList *l, size_t index);

int ll_remove(LList *l, void *data, int (*cmp)(const void *, const void *));

void ll_foreach(const LList *l, void (*fn)(void *data, size_t idx));

void ll_destroy(LList **l);


#endif