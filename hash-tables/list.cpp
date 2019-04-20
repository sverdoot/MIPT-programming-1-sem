#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list.h"

#define NDEBUG

#define OK_LIST(_list, _dump_list)                   \
    {                                                 \
    int code_er = ok_Db_List (_list, _dump_list);       \
    if (code_er)                             \
        {                                      \
                        \
        EXIT_LIST(_list, code_er, _dump_list)               \
        }                                        \
    }

#define EXIT_LIST(list, error, dump)                    \
    {                                              \
    exit_Db_List (list, error, dump);                     \
    destruct_Db_List (&list);                               \
    printf ("oops");                             \
    assert (0);                              \
    }

Node* construct_Node ()
    {
    Node* tmp = (Node*) calloc (1, sizeof (Node));
    tmp->next = tmp->prev = NULL;
    tmp->str = (char*) calloc (MAX_LEN_WORD, sizeof (char));
    return tmp;
    }

Node* destruct_Node (Node* elm)
    {
    elm->next = elm->prev = NULL;
    elm->str = NULL;
    elm = NULL;
    return elm;
    }

Db_List* construct_Db_List ()
    {
    Db_List *tmp = (Db_List*) calloc (1, sizeof (Db_List));
    tmp->size = 0;
    tmp->tail = tmp->head = construct_Node ();
    return tmp;
    }

void destruct_Db_List (Db_List **list)
    {
    Node* tmp = construct_Node ();
    assert (tmp);
    while ((*list)->head)
        {
        tmp = (*list)->head;
        (*list)->head = (*list)->head->next;
        free (tmp);
        (*list)->size--;
        }
    }

Node* pushFront (Db_List *list, char *str, FILE* dump_list)
    {
    assert (str);
    OK_LIST(list, dump_list)
    Node* temp = construct_Node ();
    strcpy (temp->str, str);
    if (list->size > 0)
        {
        temp->next = list->head;
        list->head->prev = temp;
        list->head = temp;
        }
    else
        {
        list->head = list->tail = temp;
        //list->head->next = list->tail;
        //list->tail->prev = list->head;
        }
    list->size++;
    OK_LIST(list, dump_list)
    return temp;
    }

Node* popFront (Db_List *list, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    Node *tmp = construct_Node ();
    if (list->size > 1)
        {
        list->head = list->head->next;
        list->head->prev = NULL;
        list->size--;
        }
    else if (list->size == 1)
        {
        list->head = list->tail = NULL;
        list->size = 0;
        }
    OK_LIST(list, dump_list)
    return tmp;
    }

Node* pushBack(Db_List *list, char *str, FILE* dump_list)
    {
    assert (str);
    OK_LIST(list, dump_list)
    Node* temp = construct_Node ();
    strcpy (temp->str, str);
    if (list->size > 0)
        {
        temp->prev = list->tail;
        list->tail->next = temp;
        list->tail = temp;
        }
    else if (list->size == 0)
        {
        list->head = list->tail = temp;
        //list->head->next = list->tail;
        //list->tail->prev = list->head;
        }
    list->size++;
    OK_LIST(list, dump_list)
    return temp;
    }

Node* popBack(Db_List *list, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    Node *tmp = construct_Node ();
    if (list->size > 1)
        {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
        list->size--;
        }
    else if (list->size == 1)
        {
        list->head = list->tail = NULL;
        list->size = 0;
        }
    OK_LIST(list, dump_list)
    return tmp;
    }

Node* Slow_getN (Db_List *list, int index, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    Node *tmp = NULL;
    int i;

    if (index < list->size/2)
        {
        i = 0;
        tmp = list->head;
        while (tmp && i < index)
            {
            tmp = tmp->next;
            i++;
            }
        }
    else
        {
        i = list->size - 1;
        tmp = list->tail;
        while (tmp && i > index)
            {
            tmp = tmp->prev;
            i--;
            }
        }
    return tmp;
    }

void insert_after (Db_List *list, Node *elem, char *str, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    assert (str);
    assert (elem);
    Node* tmp = (Node*) calloc (1, sizeof (Node));
    tmp->prev = elem;
    tmp->next = elem->next;
    elem->next->prev = tmp;
    elem->next = tmp;
    tmp->str = (char*) calloc (1, sizeof(char));
    strcpy (tmp->str, str);
    list->size++;
    OK_LIST(list, dump_list)
    }

void insert_before (Db_List *list, Node *elem, char *str, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    assert (str);
    assert (elem);
    Node* tmp = (Node*) calloc (1, sizeof (Node));
    tmp->next = elem;
    tmp->prev = elem->prev;
    elem->prev->next = tmp;
    elem->prev = tmp;
    tmp->str = (char*) calloc (1, sizeof(char));
    strcpy (tmp->str, str);
    list->size++;
    OK_LIST(list, dump_list)
    }

char* delete_elem (Db_List *list, Node *elem, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    char* tmp = elem->str;
    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;
    delete elem;
    list->size--;
    OK_LIST(list, dump_list)
    return tmp;
    }

Node* find_by_value (Db_List *list, char* str, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    assert (str);
    Node* tmp = construct_Node ();
    //tmp->str = (char*) calloc (1, sizeof(char));
    tmp = list->head;
    if (list->size == 0)
        return NULL;
    //if (list->size == 1)
        //return NULL;
    //int i = 0;
    while (tmp && strcmp (tmp->str, str) != 0)
        {
        //printf ("%d" ,i);
        tmp = tmp->next;
        //i++;
        }
    return tmp;
    }

void delete_all (Db_List *list, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    Node* tmp = (Node*) calloc (1, sizeof (Node));
    tmp = list->head;
    int list_size = list->size;
    for (int i = 0; i < list_size - 1; i++)
        {
        tmp = tmp->next;
        tmp->prev->str = NULL;
        delete tmp->prev;
        tmp->prev = NULL;
        list->size--;
        }
     list->tail->str = NULL;
     delete list->tail;
     list->tail->prev = NULL;
     list->size--;
     list->head->str = NULL;
     list->tail->str = NULL;
     list->head->next = NULL;
     delete tmp;
     }

char* deleteN (Db_List *list, int index, FILE* dump_list)
    {
    OK_LIST(list, dump_list)
    Node *elem = NULL;
    char *tmp = NULL;
    elem = Slow_getN (list, index, dump_list);
    if (elem == NULL)
        {
        exit(5);
        }
    if (elem->prev)
        {
        elem->prev->next = elem->next;
        }
    if (elem->next)
        {
        elem->next->prev = elem->prev;
        }
    tmp = elem->str;

    if (!elem->prev)
        {
        list->head = elem->next;
        }
    if (!elem->next)
        {
        list->tail = elem->prev;
        }

    free(elem);

    list->size--;
    OK_LIST(list, dump_list)
    return tmp;
    }

void dump_Db_List (Db_List *list, FILE* dump_list)
    {
    if (list != NULL)
        {
        fprintf (dump_list, "==DUMP==\nsize of double list is %d\n", list->size);
        Node* tmp = (Node*) calloc (1, sizeof (Node));
        tmp = list->head;
        for (int i = 0; i < list->size && tmp; i++)
            {
            fprintf (dump_list, "%d element: pointer %p, value is %s \n", i, tmp, tmp->str);
            tmp = tmp->next;
            }
        }
    else
        fprintf (dump_list, "==DUMP==\nlist doesn't exist");
    }

int ok_Db_List (Db_List *list, FILE* dump)
    {
    if (list == NULL)
        return LIST_POINTER_NULL;
    if (list->size > 0)
        {
        if (list->head == NULL)
            return LIST_HEAD_NULL;
        if (list->tail == NULL)
            return LIST_TAIL_NULL;
        }

    Node* tmp = list->head;
    for (int i = 1; i < list->size - 1; i++)
        {
        //if (!tmp->str)
            //return LIST_MISS_VALUE;
        if (!tmp->next)
            return LIST_BREAK;
        tmp = tmp->next;
        }
    //if (!tmp->str)
        //return LIST_MISS_VALUE;

    return DEFAULT;
    }

void exit_Db_List (Db_List *list, int code_er, FILE* dump)
    {
    if (code_er == DEFAULT)
        {
        }
    else
        {
        assert (dump);
        //fprintf (dump, "\n==Error==\n");
        switch (code_er)
            {
            case LIST_POINTER_NULL:
                fprintf (dump, "\tlist = NULL\n");
                break;

            //case LIST_SIZE_INVALID:
                //fprintf (dump, "\tsize of list = %d\n", list->size);
                //break;

            case LIST_HEAD_NULL:
                fprintf (dump, "\thead = %p\n", list->head);
                break;

            case LIST_TAIL_NULL:
                fprintf (dump, "\ttail = %p\n", list->tail);
                break;

            //case LIST_MISS_VALUE:
                //fprintf (dump, "\ta value is missed\n");
                //break;

            case LIST_BREAK:
                fprintf (dump, "\tthere is a break\n");
                break;

            default:
                break;
            }
        dump_Db_List (list, dump);
        }
    }









