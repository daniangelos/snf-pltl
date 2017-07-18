#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "simpl.h"

extern struct list *formula_list;
extern struct symbol_table *st;

int atomicOcc = 1;

void simplification(list *formulae){
    /* Empty list of formulae, nothing to be done */
    if(formulae == NULL) return;

    /* For each formula in the list, apply the simplifications steps
     * in all its subformulas */
    list *it;
    for (it = formulae; it != NULL; it = it->next){
        simplStep((tree*) it->element); 
    }
    return;
}

void simplStep(tree *formula){
    /* Empty formula, nothing to be done */
    if(formula == NULL) return;

    list *children;
    int op = formula->op;
    switch(op){
        case TRUE:
            truthParenting(formula);
            break;
        case FALSE:
            printf("False\n");
            falseParenting(formula);
            break;
        case NAME: /* Nothing to do, simply */ break;
        case AND:
        case OR:
        case ALWAYS:
        case NEXT:
        case SOMETIME:
        case UNLESS:
        case UNTIL:
            simplification(formula->children);
            break;
        case IMPLICATION:
        case EQUIVALENCE:
            children = formula->children;
            if(checkEquality(children, list_Tail(children))){
                /* BOTH SIDES EQUAL -> SIMPLIFY TO TRUE */
                formula->op = TRUE;
                list_Delete(&children);
            }
            else simplification(children);
            break;
        default:
            printf("Operator could not be identified\n");
            abort();
    }
}

void truthParenting(tree *formula){
    if(formula == NULL || formula->parent == NULL) return;
    tree *p = formula->parent;
    tree *tmp = NULL;
    int op = p->op;
    int child;
    switch(op){
        /*Eliminate the True node from the tree*/
        case AND: /* 46 */
        case EQUIVALENCE: 
            /*Check if it's the second child*/
            child = firstborn(p);
            if(child == TRUE) goto NONATOMIC;
            else if(child < 0) break;
            tmp = (tree*) p->children->element;
            p->op = tmp->op;
            p->children = tmp->children;
            if(p->children != NULL){
                ((tree*)p->children->element)->parent = p;
                ((tree*)list_Tail(p->children)->element)->parent = p;
            }
            if(tree_Id(tmp)) p->id = strdup(tmp->id);
            tmp->children = NULL;
            tree_Delete(&tmp);
            tree_Delete(&formula);
            break;
        case IMPLICATION:
            child = firstborn(p);
            if(child == TRUE){
NONATOMIC:      /*Here*/ tmp = (tree*) list_Tail(p->children)->element;
                p->op = tmp->op;
                p->children = tmp->children;
                if(p->children != NULL){
                    ((tree*)p->children->element)->parent = p;
                    ((tree*)list_Tail(p->children)->element)->parent = p;
                }
                if(tree_Id(tmp)) p->id = strdup(tmp->id);
                tmp->children = NULL;
                tree_Delete(&tmp);
                tree_Delete(&formula);
                if(op == AND) simplStep(p);
                break;
            }
            else if(child < 0) break;
        /* For the following cases, just replace the parenting 
         * node by True */
        case ALWAYS:
        case NEXT:
        case SOMETIME:
        case OR:
        case UNLESS:
ATOMIC:     p->op = formula->op;
            free(p->id);
            list_Delete(&(p->children));
            tree_Delete(&formula);
            if(op == OR) simplStep(p->parent);
            break;
        case UNTIL:
            child = firstborn(p);
            if(child == TRUE){
                p->op = SOMETIME;
                p->children = list_Tail(p->children);
                tree_Delete(&formula);
                break;
            }
            else if(child < 0) break;
            goto ATOMIC;
        default:
            printf("Operator could not be identified\n");
            abort();
    }
}

void falseParenting(tree *formula){
    if(formula == NULL || formula->parent == NULL) return;

    tree *p = formula->parent;
    int op = p->op;
    switch(op){
        case ALWAYS:
        case NEXT:
        case SOMETIME:
        case AND:
            p->op = formula->op;
            free(p->id);
            list_Delete(&(p->children));
            if(op == AND) simplStep(p->parent);
            break;
    }
}

int firstborn(tree *formula){
    if(formula == NULL) return -1;
    tree *firstborn = (tree*) formula->children->element;
    return firstborn->op;
}

int checkEquality(list *subf1, list *subf2){
    if(subf1 == NULL){
        if(subf2 == NULL) return 1;
        else return 0;
    }
    else if(subf2 == NULL) return 0;

    tree *child1 = (tree*) subf1->element;
    tree *child2 = (tree*) subf2->element;

    if(tree_Id(child1) && tree_Id(child2)){ 
        if (strcmp(child1->id, child2->id) != 0) return 0;
        else return 1;
    }
    
    if(child1->op == child2->op){
        int right = checkEquality(child1->children, child2->children);
        if(!right) return 0;
        int left = checkEquality(list_Tail(child1->children), 
                list_Tail(child2->children));
        if(!left) return 0;
    }
    else return 0;

    return 1;
}

