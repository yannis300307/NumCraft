#include <malloc.h>

#define LIST_CELL(t)                                                          \
    struct t##_list_cell                                                      \
    {                                                                         \
        t value;                                                              \
        t##_list_cell *next = nullptr;                                        \
    };                                                                        \
    struct t##_list                                                           \
    {                                                                         \
        t##_list_cell *last = nullptr;                                        \
        t##_list_cell *first = nullptr;                                       \
        size_t size = 0;                                                      \
    };                                                                        \
    bool push_back(t##_list *list, t value)                                   \
    {                                                                         \
        t##_list_cell *cell = (t##_list_cell *)malloc(sizeof(t##_list_cell)); \
        if (list->size == 0)                                                  \
            list->first = cell;                                               \
        cell->value = value;                                                  \
        list->last->next = cell;                                              \
        list->last = cell;                                                    \
    }                                                                         \
    t get_item(int index)                                                     \
    {                                                                         \
        /*TODO*/                                                              \
    }

LIST_CELL(int);
