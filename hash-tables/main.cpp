#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <assert.h>
#include "list.h"
#include "hash_table.h"
#include "hash functions.h"

#define CALL_HF (func_name, filein, table)                                  \
    {                                                                       \
    hash_table* table = construct_HT (filein, (int (*) (void*)) (func_name));\
    make_histogram (table, histogram);                                      \
    destruct_HT (table);                                                    \
    }
int main()
    {
    FILE* dump_dirt = fopen ("dump.txt", "w");
    assert (dump_dirt);
    fclose (dump_dirt);
    FILE* dump = fopen ("dump.txt", "a");
    assert (dump);
    FILE* input = fopen ("input.txt", "r");
    assert (input);
    FILE* histogram0 = fopen ("histogram.csv", "a");
    fclose (histogram0);
    FILE* histogram = fopen ("histogram.csv", "w");
    assert (histogram);
    hash_table* table = construct_HT (input, (int (*) (void*)) (HF_strlen), dump);
    make_histogram (table, histogram);
    destruct_HT (table);
    hash_table* table1 = construct_HT (input, (int (*) (void*)) (HF_const_0), dump);
    make_histogram (table1, histogram);
    destruct_HT (table1);
    hash_table* table2 = construct_HT (input, (int (*) (void*)) (HF_first_letter), dump);
    make_histogram (table2, histogram);
    destruct_HT (table2);
    hash_table* table3 = construct_HT (input, (int (*) (void*)) (HF_sum), dump);
    make_histogram (table3, histogram);
    destruct_HT (table3);
    hash_table* table4 = construct_HT (input, (int (*) (void*)) (HF_avg), dump);
    make_histogram (table4, histogram);
    destruct_HT (table4);

    //dump_HT (table2, dump);
    fclose (dump);
    /*FILE* dump_dirt = fopen ("dump.txt", "w");
    assert (dump_dirt);
    fclose (dump_dirt);
    FILE* dump = fopen ("dump.txt", "a");
    assert (dump);
    Db_List* list = construct_Db_List ();

    char data[7][2] = {"a", "b", "c", "t", "e", "u", "f"};
    Node* elm6 =  find_by_value (list, data[6], dump);
    printf ("%p", elm6);
    Node* elm4 = pushBack (list, data[2], dump);
    Node* elm = pushFront (list, data[0], dump);
    Node* elm3 = pushFront (list, data[1], dump);
    Node* elm2 = pushBack (list, data[3], dump);
    Node* elm5 = pushBack (list, data[4], dump);
    elm = Slow_getN (list,1 ,dump);
    //char* s = deleteN (list, 1, dump);
    //printf ("%s", s);
    insert_after (list, elm3, data[5], dump);
    elm6 =  find_by_value (list, data[6], dump);
    printf ("%p", elm6);
    dump_Db_List (list, dump);
    system ("pause");*/
    return 0;
    }


