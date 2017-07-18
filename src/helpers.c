#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "parser.tab.h"

/* List Functions */

list* list_New() {
	list* l = malloc(sizeof(list));
    l->length = 1;
	l->element = NULL;
	l->next = NULL;
	return l;
}

int list_IsEmpty(list *l) {
	return (l == NULL);
}

list* list_Tail(list *l) {
	return l->next;
}

void* list_Element(list *l) {
	return l->element;
}

void* list_FirstElement(list *l) {
	return list_Element(l);
}

list* list_PushBack(list *l, void *element) {
	list *last = NULL;
	/* find the last list element */
	for(list *it = l; !list_IsEmpty(it); it = list_Tail(it)) {
        it->length++;
		last = it;
	}
	list *newListElement = list_New();
	newListElement->element = element;
	if(last) {
		last->next = newListElement;
		return l;
	}
	else {
		return newListElement;
	}
}

list* list_List(void *element) {
	return list_PushBack(NULL, element);
}

void list_Delete(list **_l) {
    list *l = *_l;
	if(list_IsEmpty(l)) return;

	for(list *it = list_Tail(l); !list_IsEmpty(it);) {
		list *l2 = list_Tail(it);
        tree_Delete((tree**) (&l->element));
		free(it);
		it = l2;
	}

	free(*_l);
    *_l = NULL;
}

void list_Sort(list *l){
}

void list_Print(list *l){
    if(l == NULL){
        printf("empty list");
        return;
    }

	if(!list_IsEmpty(l)) {
        printf("Length: %d ", l->length);
		for(list *it = l; !list_IsEmpty(it); it = list_Tail(it)) {
			tree_Print((tree*) list_Element(it));
            printf("\n");
		}
	}
}

/* Tree Functions */

tree* tree_New() {
	tree *t = malloc(sizeof(tree));
	t->id = NULL;
	t->op = 0;
	t->children = NULL;
	t->parent = NULL;
	return t;
}

tree* tree_TreeOp(int op) {
	tree *t = tree_New();
	t->op = op;
	return t;
}

tree* tree_TreeId(char *id){
    tree *t = tree_New();
    t->id = strdup(id);
    return t;
}

void tree_SetOp(int op, tree *t){
    if(t != NULL) t->op = op;
}

void tree_SetId(char *id, tree *t){
    if(t != NULL) t->id = strdup(id);
}

list* tree_Children(tree *t) {
	return t->children;
}

tree* tree_Parent(tree *t) {
	return t->parent;
}

void tree_SetChildren(tree *t, list *children) {
	t->children = children;
	for(list *it = children; !list_IsEmpty(it); it = list_Tail(it)) {
		tree_SetParent((tree*)list_Element(it), t);
	}
}

void tree_SetParent(tree *t, tree *parent) {
	t->parent = parent;
}

void tree_AddChild(tree *t, tree *child) {
	tree_SetChildren(t, list_PushBack(tree_Children(t), child));
}

char* tree_Id(tree *t)
{
	return t->id;
}

int tree_Op(tree *t)
{
	return t->op;
}

void op_Print(int op){
    switch(op){
        case TRUE:
            printf("true");
            break;
        case FALSE:
            printf("false");
            break;
        case AND:
            printf("and");
            break;
        case OR:
            printf("or");
            break;
        case ALWAYS:
            printf("always");
            break;
        case NEXT:
            printf("next");
            break;
        case SOMETIME:
            printf("sometime");
            break;
        case UNLESS:
            printf("unless");
            break;
        case UNTIL:
            printf("until");
            break;
        case IMPLICATION:
            printf("implies");
            break;
        case EQUIVALENCE:
            printf("equivalence");
            break;
        default:
            printf("Operator could not be identified\n");
            abort();
    }
}

void tree_Print(tree *t)
{
	if(t == NULL) {
		printf("empty tree");
		return;
	}
	if(tree_Id(t)) {
		printf("%s", tree_Id(t));
	}
	else {
		op_Print(t->op);
	}

	if(!list_IsEmpty(tree_Children(t))) {
		printf("(");
		for(list *it = tree_Children(t); !list_IsEmpty(it); it = list_Tail(it)) {
			tree_Print((tree*)list_Element(it));
			if(!list_IsEmpty(list_Tail(it))) {
				printf(", ");
			}
		}
		printf(")");
	}
}

void tree_Delete(tree **t) {
	if(*t == NULL) {
		return;
	}
    free((*t)->id);
    list_Delete(&((*t)->children));
    free(*t);
    *t = NULL;
}

/*Symbol Table Functions*/

void st_InsertEntry(char *name, symbol_table **st){
    symbol_table *entry = NULL;
    char *id = strdup(name);

    HASH_FIND_STR(*st, id, entry);
    if(entry != NULL){
        entry->occurrences++;
        return;
    }
    printf("New symbol entry: %s\n", name);

    entry = malloc(sizeof(symbol_table));
    entry->id = id;
    entry->occurrences = 1;
    HASH_ADD_STR(*st, id, entry);
    return;
}

void st_Print(symbol_table *st){
    symbol_table *it;

    for(it = st; it != NULL; it = it->hh.next){
        printf("Prop symbol: %s \tOccurrences: %d\n", it->id, it->occurrences);
    }
}
