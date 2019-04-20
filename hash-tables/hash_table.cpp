#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "hash_table.h"

#define NDEBUG


const int HT_SIZE = 13;

int getfsize (FILE *filein )
    {
    assert (filein);
    fseek (filein, 0, SEEK_END);
    int fsize = ftell (filein);
    fseek (filein, 0, SEEK_SET);
    assert (fsize != -1);
    return fsize;
    }

hash_table* construct_HT (FILE* input, int (*comp) (void*), FILE* dump)
    {
    //assert (input);
    //assert (comp);
    hash_table* table = (hash_table*) calloc (1, sizeof (hash_table));
    table->size = HT_SIZE;
    table->lists = (Db_List**) calloc (table->size, sizeof (Db_List));
    int fsize = getfsize (input);
    char *buffer = (char*) calloc (fsize+1, sizeof (char));
    fread (buffer, 1, fsize, input);
    char* word = {};
    for (int i = 0; i < table->size; i++)
        table->lists[i] = construct_Db_List ();
    int i = 0;
    while ((word != NULL || i == 0) && i < fsize)
        {
        word = getword (buffer, &i);
        if (word != NULL)
            {
            int hash = (*comp) (word) % table->size;

            if (!find_by_value (table->lists[hash], word, dump))
                pushFront (table->lists[hash], word, dump);
            }
        }
    //ok_HT (table);
    return table;
    }

char* getword (char* buffer, int* i)
    {
    assert (buffer);
    assert (i);
    char* word = (char*) calloc (MAX_LEN_WORD, sizeof (char));
    char c = 0;
    while ((c = buffer[(*i)++]) == ' ');
    (*i)--;
    while (c == '\n' || c == '\t' || c == '\0')
        {
        (*i)+=1;
        c = buffer[(*i)];
        }
    if (c != '\0')
        {
        int j = 0;
        while ((c = buffer[(*i)++]) != ' ' && c != '\n' && c != '\t' && c != '\0')
            {
            word[j++] = c;
            }
        }
    if (c == '\n' || c == '\t' || c == '\0')
        (*i)+=1;
    (*i)--;
    return word;
    }

hash_table* destruct_HT (hash_table* table)
    {
    assert (table);
    for (int i = 0; i < table->size; i++)
        destruct_Db_List (&(table->lists[i]));
    free (table->lists);
    table->lists = NULL;
    table->size = -1;
    free (table);
    return table;
    }

Node* find_HT (hash_table* table, char* str, int (*comp) (void*), FILE* dump)
    {
    assert (table);
    assert (str);
    assert (comp);
    int hash = (*comp) (str) % table->size;
    Node* elm = find_by_value (table->lists[hash], str, dump);
    return elm;
    }

Node* insert_HT (hash_table* table, char* str, int (*comp) (void*), FILE* dump)
    {
    assert (table);
    assert (str);
    assert (comp);
    int hash = (*comp) (str) % table->size;
    Node* elm = NULL;
    if (!find_by_value (table->lists[hash], str, dump))
        elm = pushFront (table->lists[hash], str, dump);
    return elm;
    }

/*char* delete_HT (hash_table* table, Node* elm, int (*comp) (void*))
    {
    int hash = (*comp) (str) % table->size;
    char* str= delete_elem (table->lists[hash], elm);
    return str;
    }*/

int ok_HT (hash_table* table, FILE* dump)
    {
    assert (table);
    if (table == NULL)
        return HT_POINTER_NULL;
    if (table->size < 0)
        return HT_SIZE_INVALID;
    if (table->lists == NULL)
        return HT_LISTS_NULL;
    for (int i = 0; i < table->size; i++)
        {
        int code_er = ok_Db_List (table->lists[i], dump);
        if (code_er != DEFAULT)
            {
            fprintf (dump, "An error %d was been met in list with hash %d", code_er, i);
            exit_Db_List (table->lists[i], code_er, dump);
            return LIST_ERROR;
            }
        }
    return DEFAULT_HT;
    }

void exit_HT (hash_table* table, int code_er)
    {
    assert (table);
    if (code_er != LIST_ERROR && code_er != DEFAULT_HT)
        {
        FILE* dump = fopen ("dump.txt", "a");
        fprintf (dump, "\n==ERROR==\n");
        switch (code_er)
            {
            case HT_POINTER_NULL:
                fprintf (dump, "\tpointer on hash table == NULL\n");
                break;

            case HT_SIZE_INVALID:
                fprintf (dump, "\tsize of hash table is %d\n", table->size);
                break;

            case HT_LISTS_NULL:
                fprintf (dump, "\ttable->lists == NULL\n");
                break;

            default:
                break;

            }
        dump_HT (table, dump);
        fclose (dump);
        }
    }

void dump_HT (hash_table* table, FILE* dump)
    {
    assert (table);
    assert (dump);
    fprintf (dump, "\n=====DUMP HASH TABLE=====\n");
    for (int i = 0; i < table->size; i++)
        {
        fprintf (dump, "\thash = %d\n", i);
        dump_Db_List (table->lists[i], dump);
        }
    fprintf (dump, "\n=====END OF DUMPING HASH TABLE=====\n");
    }

void make_histogram (hash_table* table, FILE* out)
    {
    assert (table);
    assert (out);
    for (int i = 0; i < table->size; i++)
        {
        Node* tmp = (Node*) calloc (1, sizeof (Node));
        tmp = table->lists[i]->head;
        int counter = 0;
        while (tmp != table->lists[i]->tail)
            {
            counter++;
            tmp = tmp->next;
            }
        fprintf (out, "%d;%d\n", i, counter);
        delete tmp;
        }
    fprintf (out, "\n");
    }



















