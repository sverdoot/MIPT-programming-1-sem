#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <cctype>
#include "the tree.h"
#include "recurs_down.h"


const char* s = 0;


/*tnode* GetA (char** variables, int* var_num)
    {
    tnode* left = GetV (variables, var_num);

    if (*s == '=')
        s++;
    else return NULL;

    tnode* right = GetE (variables, var_num);

    return _constr_tnode (OPERATOR, Assign, left, right, NULL);
    }  */

/*tnode* GetEqu (char** variables, int* var_num)
    {
    tnode* left = GetE (variables, var_num);
    tnode* node = NULL;

    switch (*s)
        {
        case '=':
            s++;
            if (*s++ == '=')
                node = construct_tnode (EQU, OPERATOR);
            break;
        case '!':
            s++;
            if (*s++ == '=')
                node = construct_tnode (not_equ, OPERATOR);
            break;
        case '>':
            s++;
            node = construct_tnode (_more, OPERATOR);
            break;
        case '<':
            s++;
            node = construct_tnode (_less, OPERATOR);
            break;
        default:
            s--;
            return NULL;
        }
    tnode* right = GetE (variables, var_num);

    if (node && left && right)
        {
        node->left = left;
        node->right = right;
        }

    return node;
    }*/

tnode* GetV (char** variables, int* var_num)
    {
    char* var = (char*) calloc (IDENT_MAX_LEN, sizeof (char));
    int i = 0;

    if (isalpha (*s) || *s == '_')
        while ((isalnum (*s) || *s == '_') && *s != '\0')
            {
            var[i++] = *s++;
            }
    else return NULL;

    var[i] = '\0';
    for (i = 0; i < *var_num; i++)
        if (!strcmp (var, variables[i]))
            {
            return construct_tnode (i+1, VARIABLE);
            break;
            }

    if (i == *var_num)
        {
        (*var_num) += 1;
        strcpy (variables[(*var_num) - 1], (const char*)var);
        return construct_tnode (*var_num, VARIABLE);
        }
    return NULL;
    }

tnode* GetN ()
    {
    double res = 0.0;
    double fract = 0.0;
    double digit = 1.0;

    while (*s >= '0' && *s <= '9')
        res = res * 10 + *s++ - '0';


    if (*s == '.' && isdigit (*(s+1)))
        {
        s++;
        while (isdigit (*s++))
            {
            digit = digit / 10.0;
            fract = fract * 10 + *s - '0';
            }

        res = res + double (fract) * digit;
        }

    return construct_tnode (res, NUMBER);
    }

tnode* GetG0 (const char* str, char** variables)
    {
    int* var_num = (int*) calloc (1, sizeof (int));
    s = str;
    tnode* res = GetP (variables, var_num);
    printf ("%c", *s);
    assert (*s == '\0');
    return res;
    }

tnode* GetE (char** variables, int* var_num)
    {
    tnode* left = GetT (variables, var_num);
    int _operator = -1;
    tnode* right = NULL;

    while (*s == '+' || *s == '-')
        {
        if (*s++ == '+')
            {
            _operator = ADD;
            right = GetT (variables, var_num);
            left = _constr_tnode (OPERATOR, _operator, left, right, NULL);
            }
        else
            {
            _operator = SUB;
            right = GetT (variables, var_num);
            left = _constr_tnode (OPERATOR, _operator, left, right, NULL);
            }
        }

    return left;
    }

tnode* GetT (char** variables, int* var_num)
    {
    tnode* left = GetPow (variables, var_num);
    int _operator = -1;
    tnode* right = NULL;

    while (*s == '*' || *s == '/')
        {
        if (*s++ == '*')
            {
            _operator = MUL;
            right = GetPow (variables, var_num);
            left = _constr_tnode (OPERATOR, _operator, left, right, NULL);
            }
        else
            {
            _operator = DIVIDE;
            right = GetPow (variables, var_num);
            left = _constr_tnode (OPERATOR, _operator, left, right, NULL);
            }
        }

    return left;
    }

tnode* GetP (char** variables, int* var_num)
    {

    tnode* Node = NULL;

    if (isdigit (*s))
        {
        Node = GetN ();
        }
    else if (*s == '(')
        {
        s++;
        Node = GetE (variables, var_num);

        if (*s == ')')
            {
            s++;
            }
        }
    else if (isalpha (*s) || *s == '_')
        Node = GetF (variables, var_num);

    return Node;
    }
tnode* GetF (char** variables, int* var_num)
    {
    tnode* Node = NULL;
    if (*s == 'c' && *(s+1) == 'o' && *(s+2) == 's')
        {
        s +=3;
        Node = construct_tnode (COS, OPERATOR);
        Node->left = GetP (variables, var_num);
        }
    else if (*s == 's' && *(s+1) == 'i' && *(s+2) == 'n')
        {
        s +=3;
        Node = construct_tnode (SIN, OPERATOR);
        Node->left = GetP (variables, var_num);
        }
    else if (*s == 'l' && *(s+1) == 'n')
        {
        s +=2;
        Node = construct_tnode (LN, OPERATOR);
        Node->left = GetP (variables, var_num);
        }
    else
        Node = GetV (variables, var_num);

    return Node;
    }

 /*
tnode* GetOp (char ** variables, int* var_num)
    {
    tnode* node = NULL;

    if (*s == '{')
        {
        s++;
        node = GetOp (variables, var_num);

        if (*s == '}')
            {
            s++;
            return node;
            }
        }

    else if (*s == 'i' && *(s+1) == 'f' && *(s+2) == ' ')
        {
        s += 3;
        tnode* left = GetEqu (variables, var_num);
        tnode* right = GetOp (variables, var_num);
        return _constr_tnode (OPERATOR, _if, left, right, NULL);
        }
    else if (isalpha (*s) || *s == '_')
        {
        node = GetA (variables, var_num);
        return node;
        }
    else if (*s == '[')
        {
        s++;
        node = GetE (variables, var_num);
        if (*s == ']')
            {
            s++;
            return node;
            }
        else return NULL;
        }

    return node;
    } */

tnode* GetPow (char** variables, int* var_num)
    {
    tnode* left = GetP (variables, var_num);
    tnode* right = NULL;

    if (*s == '^')
        {
        s++;
        right = GetP (variables, var_num);
        return _constr_tnode (OPERATOR, POW, left, right, NULL);
        }
    else
        return left;

    return NULL;
    }
/*
tnode* GetDef (char** variables, int* var_num)
    {
    if (*s == 'd' && *(s+1) == 'e' && *(s+2) == 'f' && *(s+3) == ' ')
        {
        s += 4;
        char name[20] = {};
        int i = 0;

        while ((isalnum (*s) || *s == '_') && *s != '(')
            name[i++] = *s++;

        name[i] = '\0';

        if (*s == '(')
            {
            char** vars = (char**) calloc (10, sizeof (char*));

            while (*s != ')')
                {
                vars++ = GetV (variables, var_num);
                if (*s ! = ',')
                    return NULL;
                s++;
                }


        else
            return NULL;
             */

//Op ::= {if | Assign | '{'Op;+'}' | '['E']'}
//G0 :: = '{'Op;|E'}''\0'
//E :: = T{['+''-']T}*
//T :: = Pow{['*''/']Pow}*
//P :: = N |'('E')'
//N :: = ['0' - '9']*
//V :: = '_'/'a-Z'['a'-'Z']*
//If :: = 'if''equ''{'Op;+'}'
//Equ :: = E'==''!=''>''<'E
//Assign :: = V'='N';'
//Pow ::= P'^'P
//def


/*
F::='sin''('E')'
*/

