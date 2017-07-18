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

    printf("Formulae before: ");
    list_Print(formula_list);

    if(formula_list == NULL){
        printf("No formulae found!\n");
        return 1;
    }

    /*simplification(formula_list);*/
    snf();

    /*Printing read formulae and symbol table*/
    printf("Formulae after: ");
    list_Print(formula_list);
    printf("Symbol table: ");
    st_Print(st);

    return 0;
}
