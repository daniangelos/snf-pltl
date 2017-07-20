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
    if(l == NULL) return NULL;
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
void list_UnifyTail(list **l, list **tail, tree *element, int op){
    (*l)->next = element->children; 
    (*tail)->element = NULL;
    list_Delete(tail);
    *tail = (*l)->next;
    (*tail)->length = element->children->length;
    element->children = NULL;
    tree_Delete(&element);
    list_UnifyChildren(tail, op);
    (*l)->next = *tail;
    (*l)->length = 1 + (*tail)->length;
}

void list_UnifyHead(list **l, tree* head, int op){
    *l = head->children;
    (*l)->length += head->children->length;
    head->children = NULL;
    tree_Delete(&head);
}

int i = 0;
void list_UnifyChildren(list **l, int op){
    if(*l == NULL) return;
    tree *head = (*l)->element;
    list *tail = list_Tail(*l);
    tree *element = (tree*) tail->element;
    int right = 0;

    if(element->op == op){
        list_UnifyTail(l, &tail, element, op);
        right = 1;
    }
    if(head->op == op) {
        list_UnifyHead(l, head, op);
        list_UnifyChildren(l, op);

        // MERGE LISTS
        list *last;
        (*l)->length = 0;
        for(list *it = *l; !list_IsEmpty(it); it = list_Tail(it)) {
            (*l)->length++;
            last = it;
        }
        last->next = tail;
        (*l)->length += tail->length;
    }


    return;
}

void FrontBackSplit(list* source,
          list** frontRef, list** backRef)
{
  list* fast;
  list* slow;
  if (source==NULL || source->next==NULL)
  {
    /* length < 2 cases */
    *frontRef = source;
    *backRef = NULL;
  }
  else
  {
    slow = source;
    fast = source->next;
 
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
      fast = fast->next;
      if (fast != NULL)
      {
        slow = slow->next;
        fast = fast->next;
      }
    }
 
    /* 'slow' is before the midpoint in the list, so split it in two
      at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}

/* ## Formula Comparation ##
 * Return >0 if  a < b
 * Return 0 if  a = b
 * Return <0 if a > b  */
int formula_Compare(tree* a, tree* b){
    if(a == NULL){
        if(b == NULL) return 0;
        else return 1;
    }else if(b == NULL) return -1;

    if(a->op < b->op) return 1;
    if(a->op > b->op) return -1;

    tree *fsta = (tree*) list_Element(a->children);
    tree *fstb = (tree*) list_Element(b->children);
    tree *snda = NULL;
    tree *sndb = NULL;

    int op = a->op;
    int cmp;
    switch(op){
        case FALSE:
        case TRUE:
            return 0;
        case NAME:
            return -(strcmp(a->id, b->id));
        case AND:
        case AND_TEXT:
        case OR:
        case OR_TEXT:
            return 0;
        case EQUIVALENCE:
            snda = (tree*) list_Tail(a->children)->element;
            sndb = (tree*) list_Tail(b->children)->element;
            cmp = formula_Compare(fsta, fstb);
            if(cmp == 0) return formula_Compare(snda, sndb);
            else return cmp;
        case IMPLICATION:
        case NEXT:
        case ALWAYS:
        case SOMETIME:
        case UNTIL:
        case UNLESS:
        case NOT:
            return formula_Compare(fsta, fstb);
    }
}

list* SortedMerge(list* a, list* b)
{
  list* result = NULL;
 
  /* Base cases */
  if (a == NULL)
     return(b);
  else if (b==NULL)
     return(a);
 
  /* Pick either a or b, and recur */
  if (formula_Compare((tree*)a->element, (tree*)b->element) >= 0)
  {
     result = a;
     result->next = SortedMerge(a->next, b);
  }
  else
  {
     result = b;
     result->next = SortedMerge(a, b->next);
  }
  return(result);
}

void mergesort(list** headRef)
{
  list* head = *headRef;
  list* a;
  list* b;
 
  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
    return;
  }
 
  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b); 
 
  /* Recursively sort the sublists */
  mergesort(&a);
  mergesort(&b);
 
  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b);
}

void list_Sort(list **l){
    if(*l == NULL) return;

    tree *element = (tree*) (*l)->element;
    list *children = NULL;
    if(element != NULL){
        children = element->children;
        int op = element->op;
        if(op == AND || op == OR){
            list_UnifyChildren(&children, op);
            mergesort(&children);
            ((tree*)(*l)->element)->children = children;
        }
        if(op == EQUIVALENCE){
            mergesort(&children);
            ((tree*)(*l)->element)->children = children;
        }
    }
    
    for(list *it = children; !list_IsEmpty(it); it = list_Tail(it)) {
        list_Sort(&it);
    }
}

void list_Print(list *l){
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
        case AND_TEXT:
            printf("and");
            break;
        case OR_TEXT:
            printf("or");
            break;
        case NOT:
            printf("not");
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
            printf("Operator %d could not be identified\n", op);
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
