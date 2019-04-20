#ifndef THE_TREE_H_INCLUDED
#define THE_TREE_H_INCLUDED

#define BRACKET_OPEN(node, tex)                                                             \
    if (node->type == OPERATOR)                                                             \
        if ((node->value == ADD || node->value == SUB) && node->parent && (node->parent->value == ADD || node->parent->value == SUB || (node->parent->right && node->parent->right->value < 0))) \
                fprintf (tex, "(");                                                         \

#define BRACKET_CLOSE(node, tex)                                                            \
    if (node->type == OPERATOR)                                                             \
        if ((node->value == ADD || node->value == SUB) && node->parent && (node->parent->value == ADD || node->parent->value == SUB || (node->parent->right && node->parent->right->value < 0))) \
                fprintf (tex, ")");                                                         \

const int NUMBER = 1;
const int OPERATOR = 2;
const int VARIABLE  = 3;
const int WITH_COPY = 1;
const int WITHOUT_COPY = 2;
const int NUM_OF_OPERATORS = 40;
const int NUM_OF_VARIABLES = 10;
const int IDENT_MAX_LEN = 10;
const int PRIVATE = 1;
const int TOTAL = 2;
const int STYLE = 1;
const int SIMPLE = 0;

typedef struct tnode {
    struct tnode* parent;
    struct tnode* left;
    struct tnode* right;
    int type;
    double value;
    } tnode;

struct tree {
    tnode *root;
    };

tree* construct_tree (double value, int type);
int ok_node (tnode* node, FILE* out);
int glue_tnode (tnode* fnode, tnode* sode, FILE* out);
tnode* construct_tnode (double value, int type);
tnode* copy_subtree (tnode* root, FILE* out);
int destruct_subtree (tnode* root);
tnode* destruct_tnode (tnode* node, int mode, FILE* out);
tnode* read_part_pref (FILE* in, char** variables, char** buf, int* var_num, FILE* out);

tnode* read_expresion (FILE* in, char** variables, tnode* (*func) (void*, void*, void*, void*), FILE* out);

tnode* read_part_inf (FILE* in, char** variables, char** buf, int* var_num, FILE* out);

int write_expresion_inf (FILE* out, tnode* root, char** variables);
int write_expresion_postf (FILE* out, tnode* root, char** variables);
int write_expresion_pref (FILE* out, tnode* root, char** variables);

int getfsize (FILE *filein);
tnode* _constr_tnode (int type, double value, tnode* left, tnode* right, tnode* parent);

tnode* derivative (tnode* root, const char* var, char** variables, FILE* out, FILE* tex);
tnode* _derivative (tnode* node, int var, FILE* out, FILE* tex, char** variables);

tnode* link_tree (tnode* root, FILE* out);

tnode* simplify_tree (tnode* node, int* changes, FILE* out, FILE* tex, char** variables);
tnode* simplify_root (tnode* node, FILE* out, FILE* tex, char** variables);
tnode* math_pract (tnode* node, int* local_changes, FILE* out, FILE* tex, char** variables);

int dump_dot_tree (tnode* root, char** variables, FILE* out);
int _dump_dot (tnode* node, char** variables, int *number, FILE* out);
void dump_dot_node (tnode* node, char** variables, int number, int mode, FILE* out);

void dump_tex_start (tnode* root, FILE* out, FILE* tex);
void dump_tex_fin (tnode* root, FILE* out, FILE* tex);
void _dump_tex (tnode* root, char** variables, FILE* out, FILE* tex);
void dump_tex_expr (tnode* node, char** variables, FILE* out, FILE* tex);
void tex_text (FILE* tex);

const char c_operators[NUM_OF_OPERATORS][10] = {"+", "-", "*", "/", "sin", "cos", "^", "tg", "exp", "ln", "lg", "=", "!=", ">", "<", "if"};
enum e_operators {ADD = 0, SUB, MUL, DIVIDE, SIN, COS, POW, TG, EXP, LN, LG, EQU, NOT_EQU, MORE, LESS, IF};
const char st_operators[NUM_OF_OPERATORS][10] = {"add", "sub", "mul", "div", "sin", "cos", "pow", "tg", "exp", "ln", "lg", "="};

#endif // THE_TREE_H_INCLUDED
