#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <cctype>
#include "the tree.h"


#define OK_TREE(root, out)                      \
    {                                           \
    int err = ok_node (root, out);              \
    if (err)                                    \
        {                                       \
        printf ("%d oops!\n", err);             \
        assert (0);                             \
        }                                       \
    }

tree* construct_tree (double value, int type)
    {
    tree* tmp = (tree*) calloc (1, sizeof (tree*));
    tmp->root = construct_tnode (value, type);
    return tmp;
    }

int ok_node (tnode* node, FILE* out)
    {
    assert (node);
    assert (out);
    int error = 0;
    if (node->type == OPERATOR)
        {
        if (node->value < 0.0 || node->value >=  NUM_OF_OPERATORS)
            {
            fprintf (out, "node %p has wrong operator: %lg\n", node, node->value);
            error ++;
            }
        }
    else if (node->type != NUMBER && node->type != VARIABLE)
        {
        fprintf (out, "node %p has wrong type of value: %d\n", node, node->type);
            error ++;
        }

    int ret1 = 0;
    int ret2 = 0;

    if (node->left)
        ret1 = ok_node (node->left, out);
    if (node->right)
        ret2 = ok_node (node->right, out);

    if (error == 0 && ret1 == 0 && ret2 == 0)
        return 0;
    else
        return 1;
    }

int glue_tnode (tnode* fnode, tnode* snode, FILE* out)
    {
    OK_TREE (snode, out)
    assert (fnode);
    assert (snode);
    if (fnode->left == NULL)
        fnode->left = snode;
    else if (fnode->right == NULL)
        fnode->right = snode;
    else
        return 0;
    snode->parent = fnode;

    return 1;
    }

tnode* construct_tnode (double value, int type)
    {
    tnode* node = (tnode*) calloc (1, sizeof (tnode));

    if (type == OPERATOR)
        {
        if (value < 0.0 || value >= NUM_OF_OPERATORS)
            return NULL;
        }
    else if (type != NUMBER && type != VARIABLE)
            return NULL;

    node->value = value;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
    }

tnode* copy_subtree (tnode* root, FILE* out)
    {
    OK_TREE (root, out)
    tnode* root_cp = (tnode*) calloc (1, sizeof (tnode));

    if (root_cp == NULL)
        return NULL;
    root_cp->value = root->value;
    root_cp->type = root->type;

    if (root->left)
        {
        root_cp->left  = copy_subtree (root->left, out);
        root_cp->left->parent = root_cp;
        }
    if (root->right)
        {
        root_cp->right  = copy_subtree (root->right, out);
        root_cp->left->parent = root_cp;
        }
    OK_TREE (root, out)
    return root_cp;
    }

tnode* destruct_tnode (tnode* node, int mode, FILE* out)
    {
    OK_TREE(node, out)
    assert (node);
    if (mode == WITH_COPY)
        {
        node->value = 0.0;
        node->type = NUMBER;
        return node;
        }
    else if (mode == WITHOUT_COPY)
        {
        if (node->left == NULL && node->right == NULL)
            {
            node->value = 0.0;
            node->type = 0;
            node->parent = NULL;
            free (node);
            return NULL;
            }
        else if (node->left)
            destruct_tnode (node->left, WITHOUT_COPY, out);
        else if (node->right)
            destruct_tnode (node->right, WITHOUT_COPY, out);
        return NULL;
        }
    return NULL;
    }


int getfsize (FILE *filein)
    {
    fseek (filein, 0, SEEK_END);
    int fsize = ftell (filein);
    fseek (filein, 0, SEEK_SET);
    assert (fsize != -1);
    return fsize;
    }

tnode* read_part_pref (FILE* in, char** variables, char** adrbuf, int *var_num, FILE* out)
    {
    assert (in);
    assert (variables);
    assert (adrbuf);

    char* buf = *adrbuf;
    tnode* node  = (tnode*) calloc (1, sizeof (tnode));
    char c = 0;
    if ((c = *(buf++)) != '(')
        return NULL;
    c = *buf;
    char operat[IDENT_MAX_LEN] = {0};

    int op_len = 0;

    if ((*(buf) >= 'a' && c <= 'z' )|| *(buf) == '_' )
        {
        while (*(buf) != '(' && ((*(buf) >= 'a' && *(buf) <= 'z') || *(buf) == '_'))
            operat[op_len++] = *(buf++);

        operat[op_len] = '\0';
        }

    for (int i = 0; i < NUM_OF_OPERATORS; i++)
        if (c == c_operators[i][0])
            {
            if (op_len > 1)
                if (!strcmp (operat, c_operators[i]) )
                    {
                    node->type = OPERATOR;
                    node->value = (double)i;
                    node->left = read_part_pref (in, variables, &buf, var_num, out);

                    if (node->left)
                        node->left->parent = node;
                    if (*(buf++) != ')')
                    return NULL;
                    *adrbuf = buf;
                    return node;
                    }

                else buf = buf - op_len;

            else if (*(++buf) == '(')
                {
                node->type = OPERATOR;
                node->value = (double)i;
                node->left = read_part_pref (in, variables, &buf, var_num, out);

                if (node->left)
                    node->left->parent = node;
                node->right = read_part_pref (in, variables, &buf, var_num, out);

                if (node->right)
                    node->right->parent = node;

                if (*(buf++) != ')')
                    return NULL;

                *adrbuf = buf;
                return node;
                }
            else if (*(buf-1) == '-')
                c = *(buf);

            else
                return NULL;
            }


    if (c >= '0' && c <= '9')
        {
        char sign = *(buf-1);
        double num = c - '0';
        int fract = 0;
        double digit = 1.0;

        while ((c = *(++buf)) >= '0' && c <= '9' )
            num = num * 10.0 + c - '0';

        if (c == '.' && isdigit (*(buf+1)))
            {
            while ((c = *(++buf)) >= '0' && c <= '9' )
                {
                digit = digit / 10.0;
                fract = fract * 10 + c - '0';
                }
            num = num + double (fract) * digit;
            }

        if (sign == '-')
            num = (-1)*num;

        if (c == ')')
            {
            node->type  = NUMBER;
            node->value = (double) num;
            node->left = NULL;
            node->right = NULL;
            buf++;
            *adrbuf = buf;
            return node;
            }
        else
            return NULL;
        }

    buf--;
    if (isalpha (*buf) || *buf == '_')
        {
        int i = 0;

        for (i = 0; i < *var_num; i++)
            if (!strcmp (operat, variables[i]))
                {
                node->type = VARIABLE;
                node->value = (double) (i+1);
                break;
                }

        if (i == *var_num)
            {
            (*var_num)++;
            strcpy (variables[(*var_num) - 1], (const char*)operat);
            node->type = VARIABLE;
            node->value = (double)*var_num;
            }
        node->left = NULL;
        node->right = NULL;
        buf +=2 ;
        if (*(buf-1) == ')')
            {
            *adrbuf = buf;
            return node;
            }
        return NULL;
        }

    return NULL ;
    }

int write_expresion_pref (FILE* out, tnode* node, char** variables)
    {
    OK_TREE(node, out)
    assert (out);
    assert (node);
    assert (variables);

    fprintf (out, "(");
    if (node->type == OPERATOR)
        {
        int op = (int) node->value;
        fprintf (out, "%s", c_operators[op]);
        if (node->left)
            write_expresion_pref (out, node->left, variables);
        if (node->right)
            write_expresion_pref (out, node->right, variables);
        }
    else if (node->type == NUMBER)
        fprintf (out, "%lg", node->value);

    else if (node->type == VARIABLE)
        {
        int varnum = (int) node->value;
        fprintf (out, "%s", *(variables + varnum - 1));
        }

    fprintf (out, ")");
    return 1;
    }

tnode* read_expresion (FILE* in, char** variables, tnode* (*func) (void*, void*, void*, void*), FILE* out)
    {
    assert (in);
    assert (func);
    assert (out);
    int fsize  = getfsize (in);
    char* buf = (char*) calloc (1, fsize + 1);

    if (fread (buf, sizeof (char), fsize, in) != (unsigned int) fsize)
        return NULL;

    buf[fsize] = '\0';
    int var_num = 0;

    tnode* root = (*func) (in, variables, &buf, &var_num);

    OK_TREE (root, out)
    return root;
    }

int write_expresion_inf (FILE* out, tnode* node, char** variables)
    {
    OK_TREE(node, out)
    assert (out);
    assert (variables);

    if (node == NULL)
        return 1;

    if (node->type == OPERATOR)
        {
        fprintf (out, "(");
        if (node->value == COS || node->value == SIN || node->value == TG || node->value == EXP || node->value == LN)
            {
            int op = (int) node->value;
            fprintf (out, "%s(", c_operators[op]);
            if (node->left)
                write_expresion_inf (out, node->left, variables);
            fprintf (out, ")");
            }
        else
            {
            if (node->left)
                write_expresion_inf (out, node->left, variables);
            int op = (int) node->value;
            if (node->left && node->right)
                fprintf (out, "%s", c_operators[op]);
            else if (node->value == SUB && !node->left)
                fprintf (out, "0%s", c_operators[op]);

            if (node->right)
                write_expresion_inf (out, node->right, variables);
            }
        fprintf (out, ")");
        }
    else if (node->type == NUMBER)
        {
        if (node->value < 0.0)
            {
            fprintf (out, "(");
            fprintf (out, "%lg", node->value);
            fprintf (out, ")");
            }
        else
            fprintf (out, "%lg", node->value);
        }
    else if (node->type == VARIABLE)
        {
        int varnum = (int) node->value;
        fprintf (out, "%s", variables[varnum - 1]);
        }

    return 1;
    }

int write_expresion_postf (FILE* out, tnode* node, char** variables)
    {
    OK_TREE(node, out)
    assert (out);
    assert (variables);

    if (node == NULL)
        return 1;

    if (node->left)
        write_expresion_postf (out, node->left, variables);

    if (node->right)
        write_expresion_postf (out, node->right, variables);

    if (node->type == OPERATOR && (node->left || node->right))

        fprintf (out, "%s\n", st_operators[(int)node->value]);

    else if (node->type == NUMBER)
        {
        fprintf (out, "push %lg\n", node->value);
        }
    else if (node->type == VARIABLE)
        {
        fprintf (out, "pushr %s\n", *(variables + (int)node->value - 1));
        }

    return 1;
    }

tnode* derivative (tnode* root, const char* var, char** variables, FILE* out, FILE* tex)
    {
    OK_TREE(root, out)
    assert (root);
    assert (var);
    assert (variables);
    int i = 0;
    for (i = 0; i < NUM_OF_VARIABLES; i++)
        if (!strcmp (var, variables[i]))
            break;

    if (!strcmp(var," "))
        i = -2;

    root = _derivative (root, i+1, out, tex, variables);
    root = link_tree (root, out);
    return root;
    }

tnode* _constr_tnode (int type, double value, tnode* left, tnode* right, tnode* parent)
    {
    tnode* node  = (tnode*) calloc (1, sizeof (tnode));
    if (node == NULL)
        return NULL;
    node->type  = type;
    node->value = value;
    node->left = left;
    node->right = right;
    node->parent = parent;
    return node;
    }

tnode* _derivative (tnode* node, int var, FILE* out, FILE* tex, char** variables)
    {
    OK_TREE(node, out)
    if (node == NULL)
        return NULL;

    tnode* derivat = NULL;
    tnode* copy_node = copy_subtree (node, out);

    int value = (int) node->value;

    switch (node->type)
        {
        case OPERATOR:
            switch (value)
                {
                case EQU:
                    derivat = _constr_tnode (OPERATOR, EQU, copy_subtree (node->left, out),
                                                            _derivative (node->right, var, out, tex, variables), node->parent);
                    break;

                case ADD:
                    derivat = _constr_tnode (OPERATOR, ADD, _derivative (node->left, var, out, tex, variables),
                                                            _derivative (node->right, var, out, tex, variables), node->parent);
                    break;

                case SUB:
                    derivat = _constr_tnode (OPERATOR, SUB, _derivative (node->left, var, out, tex, variables),
                                            _derivative (node->right, var, out, tex, variables), node->parent);
                    break;

                case MUL:
                    {
                    tnode* node1 = _constr_tnode (OPERATOR, MUL, _derivative (node->left, var, out, tex, variables),
                                                                 copy_subtree (node->right, out), NULL);
                    tnode* node2 = _constr_tnode (OPERATOR, MUL, copy_subtree (node->left, out),
                                                                 _derivative (node->right, var, out, tex, variables), NULL);
                    derivat = _constr_tnode (OPERATOR, ADD, node1, node2, node->parent);
                    break;
                    }

                case DIVIDE:
                    {
                    tnode* node1 = _constr_tnode (OPERATOR, MUL, _derivative (copy_subtree (node->left, out), var, out, tex, variables),
                                                                 _constr_tnode (OPERATOR, POW, copy_subtree (node->right, out),
                                                                 _constr_tnode (NUMBER, -1, NULL, NULL, NULL), NULL), NULL);
                    tnode* node2 = _constr_tnode (OPERATOR, MUL, copy_subtree (node->left, out),
                                                                 _derivative (_constr_tnode (OPERATOR, POW, copy_subtree (node->right, out),
                                                                            construct_tnode (-1, NUMBER), NULL), var, out, tex, variables), NULL);
                    derivat = _constr_tnode (OPERATOR, ADD, node1, node2, node->parent);
                    }
                    break;
                case SIN:
                    {
                    tnode* node1 =  _constr_tnode (OPERATOR, COS, copy_subtree (node->left, out), NULL, NULL);
                    derivat = _constr_tnode (OPERATOR, MUL, node1, _derivative (node->left, var, out, tex, variables), NULL);
                    break;
                    }
                case COS:
                    {
                    tnode* node2  = _constr_tnode (OPERATOR, SIN, copy_subtree (node->left, out), NULL, NULL);
                    tnode* node1 = _constr_tnode  (OPERATOR, MUL, node2, _derivative (node->left, var, out, tex, variables), NULL);
                    derivat = _constr_tnode (OPERATOR, MUL, node1, _constr_tnode (NUMBER, -1, NULL, NULL, NULL), node->parent);
                    break;
                    }
                case POW:
                    {
                    if (node->right->type == NUMBER)
                        derivat = _constr_tnode (OPERATOR, MUL, _constr_tnode (OPERATOR, MUL, _constr_tnode (NUMBER, node->right->value, NULL, NULL, NULL),
                                                                                              _constr_tnode (OPERATOR, POW, copy_subtree (node->left, out),
                                                                                                            construct_tnode (node->right->value - 1, NUMBER), NULL), NULL),
                                _derivative (copy_subtree (node->left, out), var, out, tex, variables), NULL);
                    else
                        {
                        tnode* node1 = _constr_tnode (OPERATOR, MUL, copy_subtree (node->right, out),
                                                                     _constr_tnode (OPERATOR,  LN, copy_subtree (node->left, out),
                                                                                                   NULL, NULL), NULL);
                        derivat = _derivative (_constr_tnode (OPERATOR, EXP, node1, NULL, NULL), var, out, tex, variables);
                        }
                    break;
                    }
                case EXP:
                    derivat = _constr_tnode (OPERATOR, MUL, _constr_tnode (OPERATOR, EXP, copy_subtree (node->left, out), NULL, node),
                                                            _derivative (node->left, var, out, tex, variables), node->parent);
                    break;
                case LN:
                    derivat = _constr_tnode (OPERATOR, DIVIDE, _derivative (copy_subtree (node->left, out), var, out, tex, variables),
                                                               node->left, node->parent);
                    break;
                default:
                    break;
                }
            break;
        case NUMBER:
            derivat = _constr_tnode (NUMBER, 0, NULL, NULL, node->parent);
            break;
        case VARIABLE:
            {
            tnode* node1 = copy_subtree (node, out);

            if (var == value || var == -1)
                {
                node1->value = 1.0;
                node1->type = NUMBER;
                }
            else
                {
                node1->value = 0.0;
                node1->type = NUMBER;
                }
            derivat = node1;
            break;
            }
        default:
            break;
        }

    fprintf (tex, "\ndifferentiate this expression\n");
    dump_tex_expr (copy_node, variables, out, tex);
    fprintf (tex, "\nget the following\n");
    dump_tex_expr (derivat, variables, out, tex);
    free (copy_node);

    OK_TREE(node, out)
    return derivat;
    }

tnode* simplify_root (tnode* node, FILE* out, FILE* tex, char** variables)
    {
    OK_TREE(node, out)

    int changes = 1;

    while (changes != 0)
        {
        changes = 0;
        node = simplify_tree (node, &changes, out, tex, variables);
        }

    link_tree (node, out);

    OK_TREE(node, out)
    return node;
    }

tnode* simplify_tree (tnode* node, int *changes, FILE* out, FILE* tex, char** variables)
    {
    OK_TREE(node, out)

    int local_changes = 0;
    tnode* copy_node = copy_subtree (node, out);

    if (node->type == OPERATOR)
        {

        node = math_pract (node, &local_changes, out, tex, variables);

        if (node->left && node->left->type == NUMBER && (node->left->value == 1.0 || node->left->value == 0.0))
            {
            if (node->left->value == 0.0)
                {
                if (node->value == MUL || node->value == DIVIDE)
                    {
                    local_changes++;
                    *(changes) += local_changes;
                    return construct_tnode (0.0, NUMBER);
                    }
                else if (node->value == ADD )
                    {
                    local_changes++;
                    *(changes) = local_changes;
                    node = node->right;
                    }

                }
            else if (node->value == MUL)
                {
                local_changes++;
                node = node->right;
                }
            else if (node->value == POW)
                {
                local_changes++;
                node = construct_tnode (1.0, NUMBER);
                }
            }

        if (node->right && node->right->type == NUMBER && (node->right->value == 1.0 || node->right->value == 0.0))
            {
            if (node->right->value == 0.0)
                {
                if (node->value == MUL)
                    {
                    local_changes++;
                    *(changes) += local_changes;
                    return construct_tnode (0.0, NUMBER);
                    }

                else if (node->value == ADD || node->value == SUB)
                    {
                    local_changes++;
                    node = node->left;
                    }
                }
            else if (node->value == MUL || node->value == DIVIDE || node->value == POW)
                {
                local_changes++;
                node = node->left;
                }
            }
        }
    if (local_changes != 0)
        {
        fprintf (tex, "\nsimplify this expression\n");
        dump_tex_expr (copy_node, variables, out, tex);
        fprintf (tex, "\nget the following\n");
        dump_tex_expr (node, variables, out, tex);
        }

    free (copy_node);

    if (node->left)
        node->left = simplify_tree (node->left, &local_changes, out, tex, variables);

    if (node->right)
        node->right = simplify_tree (node->right, &local_changes, out, tex, variables);


    *(changes) += local_changes;

    OK_TREE(node, out)
    return node;
    }

tnode* math_pract (tnode* node, int* local_changes, FILE* out, FILE* tex, char** variables)
    {
    OK_TREE(node, out)

    if (node->left && node->left->type == NUMBER && (!node->right || node->right->type == NUMBER))
        {
        if (!node->right)
            node->right = construct_tnode (0.0, NUMBER);

        node->type = NUMBER;
        int value = node->value;

        switch (value)
            {
            case ADD:
                node->value = node->left->value + node->right->value;
                node->left = destruct_tnode (node->left, WITHOUT_COPY, out);
                node->right = destruct_tnode (node->right, WITHOUT_COPY, out);
                *local_changes += 1;
                break;

            case SUB:
                node->value = node->left->value - node->right->value;
                node->left = destruct_tnode (node->left, WITHOUT_COPY, out);
                node->right = destruct_tnode (node->right, WITHOUT_COPY, out);
                *local_changes += 1;
                break;

            case MUL:
                node->value = node->left->value * node->right->value;
                node->left = destruct_tnode (node->left, WITHOUT_COPY, out);
                node->right = destruct_tnode (node->right, WITHOUT_COPY, out);
                *local_changes += 1;
                break;

            case DIVIDE:
                if (node->right->value != 0)
                    {
                    node->value = node->left->value / node->right->value;
                    node->left = destruct_tnode (node->left, WITHOUT_COPY, out);
                    node->right = destruct_tnode (node->right, WITHOUT_COPY, out);
                    *local_changes += 1;
                    }
                else
                    fprintf (out, "\ncan't divide on 0\n");
                break;

            case POW:
                node->value = pow (node->left->value, node->right->value);
                node->left = destruct_tnode (node->left, WITHOUT_COPY, out);
                node->right = destruct_tnode (node->right, WITHOUT_COPY, out);
                *local_changes += 1;
                break;

            default:
                break;
            }
        }

    return node;
    }



tnode* link_tree (tnode* node, FILE* out)
    {
    OK_TREE(node, out)

    if (node->left)
        {
        node->left->parent = node;
        node->left = link_tree (node->left, out);
        }
    if (node->right)
        {
        node->right->parent = node;
        node->right = link_tree (node->right, out);
        }

    OK_TREE(node, out)
    return node;
    }

int dump_dot_tree (tnode* node, char** variables, FILE* out)
    {
    int res = 1;
    int number = 0;
    OK_TREE(node, out)
    fprintf (out, "digraph EXPRESSION {\n");
    dump_dot_node (node, variables, number, STYLE, out);
    if (node->left || node->right)
        res = _dump_dot (node, variables, &number, out);
    fprintf (out, "\n}");
    return res;
    }

int _dump_dot (tnode* node, char** variables, int *number, FILE* out)
    {
    OK_TREE(node, out)
    int num = *number;

    if (node->left)
        {
        *number += 1;
        dump_dot_node (node->left, variables, *number, STYLE, out);
        dump_dot_node (node, variables, num, SIMPLE, out);
        fprintf (out, "-> ");
        dump_dot_node (node->left, variables,  *number, SIMPLE, out);
        fprintf (out, "[color=brown]\n");
        _dump_dot (node->left, variables, number, out);
        }

    if (node->right)
        {
        *number += 1;
        dump_dot_node (node->right, variables, *number, STYLE, out);
        dump_dot_node (node, variables, num, SIMPLE, out);
        fprintf (out, "-> ");
        dump_dot_node (node->right, variables, *number, SIMPLE, out);
        fprintf (out, "[color=brown]\n");
        _dump_dot (node->right, variables, number, out);
        }

    return 1;
    }

void dump_dot_node (tnode* node, char** variables, int number, int mode, FILE* out)
    {
    OK_TREE(node, out)
    int value = (int) node->value;

    if (node->type == NUMBER)
        fprintf (out, "\"#%d \\n %lg \"", number, node->value);
    else if (node->type == OPERATOR)
        fprintf (out, "\"#%d \\n %s\"", number, c_operators [(int) node->value]);
    else
        fprintf (out, "\"#%d \\n %s \"", number, variables [value - 1]);

    if (mode == STYLE)
        {
        char red[10] = "red\0";
        char green[10] = "green\0";
        char blue[10] = "blue\0";
        char* color = NULL;
        if (node->type == OPERATOR)
            color = red;
        else if (node->type == VARIABLE)
            color = green;
        else
            color = blue;

        fprintf (out, "[shape=box,style=filled,color=%s]", color);
        }
    }


void dump_tex_start (tnode* root, FILE* out, FILE* tex)
    {
    OK_TREE(root, out)
    assert (out);
    assert (tex);

    fprintf(tex, "\\documentclass[12pt,a4paper,landscape]{scrartcl}\n\\usepackage{euscript}\n\\usepackage{mathrsfs}\n\\usepackage{cmap}\n\\usepackage[T2A]{fontenc}\n\\usepackage[utf8]{inputenc}\n\\usepackage[english,russian]{babel}\n\\usepackage{graphicx}\n\\usepackage{amsmath}\n\\author{Lagutin,677}\n\\title{REPORT ON TAKING DERIVATIVE}\n\\begin{document}\n\\maketitle\\begin{center}\n");
    }

void dump_tex_fin (tnode* root,  FILE* out, FILE* tex)
    {
    OK_TREE(root, out)
    assert (out);
    assert (tex);

    fprintf(tex, "\n\\end{center}\n\\end{document}");
    }

void dump_tex_expr (tnode* node, char** variables, FILE* out, FILE* tex)
    {
    OK_TREE(node, out)
    assert (out);
    assert (tex);
    assert (variables);

    fprintf (tex, "\n\\[");
    _dump_tex (node, variables, out, tex);
    fprintf (tex, "\\]");
    }


void _dump_tex (tnode* node, char** variables, FILE* out, FILE* tex)
    {
    OK_TREE(node, out)
    assert (out);
    assert (tex);
    assert (variables);

    if (node->type == NUMBER)
        if (node->value >= 0)
            fprintf (tex, "%lg", node->value);
        else
            fprintf (tex, "{(%lg)}", node->value);
    else if (node->type == VARIABLE)
        fprintf (tex, "%s", variables[(int)node->value - 1]);
    else if (node->type == OPERATOR)
        {
        fprintf (tex, "{");

        switch ((int)node->value)
            {
            case ADD:
                BRACKET_OPEN(node, tex);
                _dump_tex (node->left, variables, out, tex);
                fprintf (tex, "+");
                _dump_tex (node->right, variables, out, tex);
                BRACKET_CLOSE(node, tex);
                break;
            case SUB:
                BRACKET_OPEN(node, tex);
                _dump_tex (node->left, variables, out, tex);
                fprintf (tex, "-");
                _dump_tex (node->right, variables, out, tex);
                BRACKET_CLOSE(node, tex);
                break;
            case MUL:
                if (node->right->value >= 0)
                    {
                    fprintf (tex, "{");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "}\\cdot{");
                    _dump_tex (node->right, variables, out, tex);
                    fprintf (tex, "}");
                    }
                else
                    {
                    fprintf (tex, "({");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "})\\cdot{");
                    _dump_tex (node->right, variables, out, tex);
                    fprintf (tex, "}");
                    }
                break;
            case DIVIDE:
                fprintf (tex, "\\frac{");
                _dump_tex (node->left, variables, out, tex);
                fprintf (tex, "}{");
                _dump_tex (node->right, variables, out, tex);
                fprintf (tex, "}");
                break;
            case POW:
                if (node->left->type == OPERATOR && (node->left->value == SIN || node->left->value == COS || node->left->value == LN))
                    {
                    fprintf (tex, "(");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, ")");
                    fprintf (tex, "^");
                    _dump_tex (node->right, variables, out, tex);
                    }
                else
                    {
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "^");
                    _dump_tex (node->right, variables, out, tex);
                    }
                break;
            case SIN:
                if (node->left->type == OPERATOR)
                    {
                    fprintf (tex, "\\sin({");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "})");
                    }
                else
                    {
                    fprintf (tex, "\\sin{");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "}");
                    }
                break;
            case COS:
                if (node->left->type == OPERATOR)
                    {
                    fprintf (tex, "\\cos({");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "})");
                    }
                else
                    {
                    fprintf (tex, "\\cos{");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "}");
                    }
                break;
            case TG:
                if (node->left->type == OPERATOR)
                    {
                    fprintf (tex, "\\tan({");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "})");
                    }
                else
                    {
                    fprintf (tex, "\\tan{");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "}");
                    }
                break;
            case EXP:
                fprintf (tex, "\\exp({");
                _dump_tex (node->left, variables, out, tex);
                fprintf (tex, "})");
                break;
            case LN:
                if (node->left->type == OPERATOR)
                    {
                    fprintf (tex, "\\ln({");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "})");
                    }
                else
                    {
                    fprintf (tex, "\\ln{");
                    _dump_tex (node->left, variables, out, tex);
                    fprintf (tex, "}");
                    }
                break;
            case EQU:
                _dump_tex (node->left, variables, out, tex);
                fprintf (tex, "=");
                _dump_tex (node->right, variables, out, tex);
                break;
            default :
                break;
            }
        fprintf (tex, "}");
        }
    }




