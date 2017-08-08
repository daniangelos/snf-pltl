#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "snf.h"
#include "simpl.h"

#define ON 1
#define OFF 0

/* global variables for execution options */
int ruletracker = OFF;

extern FILE *yyin;
extern struct list *formula_list;
extern struct symbol_table *st;
extern int yyparse(void);
extern int yy_scan_string(const char *);

void yyerror(const char *error)
{
	printf("Error: %s\n", error);
	abort();
}

int main(int argc, char** argv) {

    int i = 1;

    while (i < argc){
        if(!strcmp(argv[i], "-ruletracker")) ruletracker = ON;
        else if (!strcmp(argv[i],"-i")) {
            yyin=fopen(argv[++i], "r");
            if (yyin == NULL) {
                printf("\n Input file not found: %s\n",argv[i]);
                return 1;
            }
        }
        else if (!strcmp(argv[i],"-f")) {
            char *formula =  malloc(snprintf(NULL, 0, "%s", argv[++i]) + 1);
            sprintf(formula,"%s",argv[i]);
            yy_scan_string(formula);
            free(formula);
        }
        else {
            printf("\n Unknown flag: %s\n",argv[i]);
            return 1;
        }
        i++;
    }

	yyparse();
    if(formula_list == NULL){
        printf("No formulae found!\n");
        return 1;
    }


    printf("Initial formulae:\n");
    list_Print(formula_list);

    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    do{
        simplified = 0;
        list_Sort(&formula_list);
        mergesort(&formula_list);
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
