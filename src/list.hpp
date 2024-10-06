#ifndef LIST_HPP
#define LIST_HPP

#include <malloc.h>

#define LIST_CELL(t)                                                          \
    struct t##_list_cell                                                      \
    {                                                                         \
        t value;                                                              \
        t##_list_cell *next = NULL;                                           \
    };                                                                        \
    struct t##_list                                                           \
    {                                                                         \
        t##_list_cell *last = NULL;                                           \
        t##_list_cell *first = NULL;                                          \
        int size = 0;                                                         \
    };                                                                        \
    static inline bool push_back(t##_list *list, t value)                     \
    {                                                                         \
        t##_list_cell *cell = (t##_list_cell *)malloc(sizeof(t##_list_cell)); \
        if (cell == NULL)                                                     \
            return false;                                                     \
        cell->value = value;                                                  \
        if (list->size == 0)                                                  \
        {                                                                     \
            list->first = cell;                                               \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            list->last->next = cell;                                          \
        }                                                                     \
        list->last = cell;                                                    \
        list->size += 1;                                                      \
        return true;                                                          \
    }                                                                         \
    struct t##_list_iterator                                                  \
    {                                                                         \
        t##_list *list;                                                       \
        int cursor = 0;                                                       \
        t##_list_cell *current = list->first;                                 \
    };                                                                        \
    static inline t *iterator_next(t##_list_iterator *iterator)               \
    {                                                                         \
        if (iterator->cursor >= iterator->list->size)                         \
            return NULL;                                                      \
        t##_list_cell *current = iterator->current;                           \
        iterator->current = iterator->current->next;                          \
        iterator->cursor += 1;                                                \
        return &(current->value);                                             \
    }                                                                         \
    static inline t *front(t##_list *list)                                    \
    {                                                                         \
        if (list->first == NULL)                                              \
            return NULL;                                                      \
        return &(list->first->value);                                         \
    }                                                                         \
    static inline t pop_front(t##_list *list)                                 \
    {                                                                         \
        t value = list->first->value;                                         \
        list->first = list->first->next;                                      \
        return value;                                                         \
    }

#endif