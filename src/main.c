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

    printf("Initial formulae:\n");
    list_Print(formula_list);
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    list_Sort(&formula_list);
    mergesort(&formula_list);
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Formulae after sorting:\n");
    list_Print(formula_list);

    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    do{
        simplified = 0;
        simplification(formula_list);
    } while(simplified && !list_IsEmpty(formula_list));

    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Formulae after simplification:\n");
    list_Print(formula_list);

    /*printf("~~~~~~~~~~~~~~~~~~~~~~~\n");*/
    /*snf(formula_list);*/
    /*printf("~~~~~~~~~~~~~~~~~~~~~~~\n");*/
    /*printf("Formulae in normal form:\n");*/
    /*list_Print(formula_list);*/

    printf("Symbol table: ");
    st_Print(st);

    list_Delete(&formula_list);
    st_Delete(&st);

    return 0;
}
