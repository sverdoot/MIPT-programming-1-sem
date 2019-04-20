#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <clocale>
#include "the tree.h"
#include "recurs_down.h"

int main()
    {
    char** variables = (char**) calloc (NUM_OF_VARIABLES, sizeof (char*));
    for (int i = 0; i < NUM_OF_VARIABLES; i++)
        variables[i] = (char*) calloc (IDENT_MAX_LEN, sizeof (char));

    FILE* in = fopen ("in.txt", "r");
    FILE* out = fopen ("out.txt", "w");
    FILE* dump_dot = fopen ("C:dump_dot1.gv", "w");
    FILE* dump_tex = fopen ("C:\\Users\\Eugenii\\Documents\\deriv_tex.tex", "w");

    setlocale(LC_CTYPE, "Russian");

    //tnode* root = read_expresion (in, variables, (tnode* (*) (void*, void*, void*, void*)) (read_part_pref), out);
    //tex_text (dump_tex);
    const char* var = "x";
    //root = simplify_tree (root, out);
    tnode* root = GetG0 ("(ln((exp^(x^2-exp))^(1/3))", variables);
    dump_tex_start (root, out, dump_tex);
    dump_tex_expr (root, variables, out, dump_tex);
    tnode* root1 = copy_subtree (root, out);
    root = derivative (root, var, variables, out, dump_tex);
    dump_tex_expr (root, variables, out, dump_tex);
    root = simplify_root (root, out, dump_tex, variables);
    root = link_tree (root, out);
    write_expresion_inf (out, root, variables);
    dump_tex_expr (root, variables, out, dump_tex);
    dump_dot_tree (root, variables, dump_dot);
    dump_tex_fin (root, out, dump_tex);
    system("start C:\\Users\\Eugenii\\Documents\\cb\\tree\\tree\\Новая_папка\\release\\bin\\gvedit.exe");
    system("start C:\\Users\\Eugenii\\Documents\\cb\\tree\\tree\\tex.lnk");
    for (int i = 0; i < NUM_OF_VARIABLES; i++)
        free (variables[i]);
    free (variables);
    fclose (in);
    fclose (out);
    return 0;
    }
