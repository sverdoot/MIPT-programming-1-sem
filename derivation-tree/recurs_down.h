#ifndef RECURS_DOWN_H_INCLUDED
#define RECURS_DOWN_H_INCLUDED
#include "the tree.h"


tnode* GetG0 (const char* str, char** variables);
tnode* GetE (char** variables, int* var_num);
tnode* GetN ();
tnode* GetT (char** variables, int* var_num);
tnode* GetP (char** variables, int* var_num);
tnode* GetV (char** variables, int* var_num);
tnode* GetEqu (char** variables, int* var_num);
tnode* GetA (char** variables, int* var_num);
tnode* GetOp (char ** variables, int* var_num);
tnode* GetPow (char** variables, int* var_num);
tnode* GetF (char** variables, int* var_num);


#endif // RECURS_DOWN_H_INCLUDED
