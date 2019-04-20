#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

const int MAX_LEN_WORD = 1000;

typedef struct _Node
    {
    char *str = NULL;
    struct _Node *next = NULL;
    struct _Node *prev = NULL;
    } Node;

typedef struct _Db_List
    {
    int size = 0;
    Node *head = NULL;
    Node *tail = NULL;
    } Db_List;

Node* destruct_Node (Node* elm);
Node* construct_Node ();
Db_List* construct_Db_List ();
void destruct_Db_List (Db_List **list);
Node* pushFront (Db_List *list, char *data, FILE* dump_list);
Node* popFront (Db_List *list, FILE* dump_list);
Node* pushBack(Db_List *list, char *str, FILE* dump_list);
Node* popBack(Db_List *list, FILE* dump_list);
Node* Slow_getN (Db_List *list, int index, FILE* dump_list);
char* deleteN (Db_List *list, int index, FILE* dump_list);
void dump_Db_List (Db_List *list, FILE* dump_list);
void insert_after (Db_List *list, Node *elem, char *str, FILE* dump_list);
void insert_before (Db_List *list, Node *elem, char *str, FILE* dump_list);
char* delete_elem (Db_List *list, Node *elem, FILE* dump_list);
Node* find_by_value (Db_List *list, char* str, FILE* dump_list);
void delete_all (Db_List *list, FILE* dump_list);
int ok_Db_List (Db_List *list, FILE* dump);
void exit_Db_List (Db_List *list, int code_error, FILE* dump);

enum list_errors {DEFAULT, LIST_POINTER_NULL, LIST_SIZE_INVALID,
                LIST_HEAD_NULL, LIST_TAIL_NULL, LIST_MISS_VALUE,
                LIST_BREAK};

#endif // LIST_H_INCLUDED


