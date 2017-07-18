#ifndef BASIC_H
#define BASIC_H 

#include "uthash.h"


/* singly linked list */
typedef struct list list;
struct list{
    formula *element;
    list *next;
};

/* formulae structure */
typedef struct formula formula;
struct formula{
    int op;
    char *prop;
    list *children;
    formula *parent;
};

/* symbol table */
struct symbol_table{
    char *id;
    int occurrences;
    UT_hash_handle hh;
};

/* Symbol Table Functions */
void st_InsertEntry(char *name, symbol_table **st);
void st_Print(symbol_table *st);

#endif
