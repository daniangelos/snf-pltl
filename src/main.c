#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "snf.h"
#include "simpl.h"

extern FILE *yyin;
extern struct list *formula_list;
extern struct symbol_table *st;
extern int yyparse(void);

void yyerror(const char *error)
{
	printf("Error: %s\n", error);
	abort();
}

int main(int argc, char** argv) {

	yyparse();

    if(formula_list == NULL){
        printf("No formulae found!\n");
        return 1;
    }

    printf("Formulae before:\n");
    list_Print(formula_list);
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");

    list_Sort(&formula_list);
    mergesort(&formula_list);
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Formulae after sorting:\n");
    list_Print(formula_list);
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    simplification(formula_list);
    snf();

    /*[>Printing read formulae and symbol table<]*/
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Formulae after simplification:\n");
    list_Print(formula_list);
    printf("Symbol table: ");
    st_Print(st);

    return 0;
}
