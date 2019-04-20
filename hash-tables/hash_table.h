#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED
#include "list.h"
typedef struct _hash_table
    {
    int size;
    Db_List** lists;
    }hash_table;

hash_table* construct_HT (FILE* input, int (*comp) (void*), FILE* dump);
hash_table* destruct_HT (hash_table* table);
int ok_HT (hash_table* table, FILE* dump);
void dump_HT (hash_table* table, FILE* dump);
Node* find_HT (hash_table* table, char* str, int (*comp) (void*), FILE* dump);
Node* insert_HT (hash_table* table, char* str, int (*comp) (void*), FILE* dump);
char* delete_HT (hash_table* table, Node* elm, int (*comp) (void*));
hash_table* delete_all_HT (hash_table* table);
int getfsize (FILE *filein );
char* getword (char* buffer, int* counter);
void exit_HT (hash_table* table, int code_er);
void make_histogram (hash_table* table, FILE* out);

enum HT_errors {DEFAULT_HT, HT_POINTER_NULL, HT_SIZE_INVALID, HT_LISTS_NULL,
                LIST_ERROR};

#endif // HASH_TABLE_H_INCLUDED
