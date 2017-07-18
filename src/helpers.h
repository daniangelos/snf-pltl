/************************************************************
 *    Copyright (C) 2017                                    *
 *                                                          *
 *    This program is free software; you can redistribute   *
 *    it and/or modify it under the terms of the GNU        *
 *    General Public License as published by the Free       *
 *    Software Foundation; either version 3 of the License, *
 *    or (at your option) any later version.                *
 *                                                          *
 *    This program is distributed in the hope that it will  *
 *    be useful, but WITHOUT ANY WARRANTY; without even     *
 *    the implied warranty of MERCHANTABILITY or FITNESS    *
 *    FOR A PARTICULAR PURPOSE.  See the GNU General Public *
 *    License for more details.                             *
 *                                                          *
 *    You should have received a copy of the GNU General    *
 *    Public License along with this program; if not, see   *
 *    <http://www.gnu.org/licenses/>.                       *
 ************************************************************/
#ifndef HELPERS_H
#define HELPERS_H

#include "uthash.h"

typedef struct tree tree;
typedef struct list list;
typedef struct symbol_table symbol_table;

/* singly linked list */
/* the empty list is represted by the null pointer */
struct list {
    int length;
	void *element;
	list *next;
};


/* string tree */
struct tree {
	char *id;
	int op;
	list *children;
	tree *parent;
};

/* symbol table */
struct symbol_table{
    char *id;
    int occurrences;
    UT_hash_handle hh;
};

/* List Functions */
list* list_New() ;
int list_IsEmpty(list *l) ;
list* list_Tail(list *l) ;
void* list_Element(list *l) ;
void* list_FirstElement(list *l) ;
list* list_PushBack(list *l, void *element) ;
list* list_List(void *element) ;
void list_Delete(list **_l) ;
void list_Sort(list *l);
void list_Print(list *l);

/* Tree Functions */
tree* tree_New() ;
tree* tree_TreeOp(int op) ;
tree* tree_TreeId(char *id);
void tree_SetOp(int op, tree *t);
void tree_SetId(char *id, tree *t);
char* tree_Id(tree *t);
list* tree_Children(tree *t);
tree* tree_Parent(tree *t);
void tree_SetChildren(tree *t, list *children);
void tree_SetParent(tree *t, tree *parent) ;
void tree_AddChild(tree *t, tree *child) ;
void tree_Print(tree *t);
void tree_Delete(tree **t);

/* Symbol Table Functions */
void st_InsertEntry(char *name, symbol_table **st);
void st_Print(symbol_table *st);

#endif //HELPERS_H
