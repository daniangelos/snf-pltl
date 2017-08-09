#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "simpl.h"

extern struct list *formula_list;
extern struct symbol_table *st;


void simplification(list *formulae){
    /* Nothing to be done */
    if(formulae == NULL) return;


    /* For each formula in the list, apply the simplifications steps */
    list *it = formulae;
    list *prev = NULL;
    while(!list_IsEmpty(it)){
        tree *element = (tree*) list_Element(it);
        simplStep(&element, &prev, &it);
        prev = it;
        it = list_Tail(it);
    }

    return;
}

void simplStep(tree **formula, list **prev, list **pos){
    /* Empty formula, nothing to be done */
    if(*formula == NULL) return;

    int op = (*formula)->op;
    switch(op){
        case TRUE:
            simplified = 1;
            truthSimplification(formula, prev, pos);
            break;
        case FALSE:
            simplified = 1;
            falseSimplification(formula, prev, pos);
            break;
        case NAME: /* Nothing to do, simply */ 
            break;
        case AND:
        case OR:
            checkUnarity(*formula);
        case UNLESS:
        case UNTIL:
        case IMPLICATION:
        case EQUIVALENCE:
            findRepeated(op, *formula, (*formula)->children);
        case NEXT:
        case NOT:
            simplification((*formula)->children);
            break;
        case ALWAYS:
            alwaysSimplification(formula, pos);
            simplification((*formula)->children);
            break;
        case SOMETIME:
            sometimeSimplification(formula, pos);
            simplification((*formula)->children);
            break;
        default:
            printf("Operator %d could not be identified\n", op);
            abort();
    }
}

void truthSimplification(tree **formula, list **prev, list **pos){
    if(*formula == NULL) return;
    if((*formula)->parent == NULL) {
        simplified = 0;
        return;
    }

    tree *p = (*formula)->parent;
    tree *elem = NULL;
    int p_op = p->op;
    int op = (*formula)->op;
    switch(p_op){
        case AND:
            /* Remove true node from tree */
            elem = *formula;
            while(elem->op == op && !list_IsEmpty(*pos)){
                list_DeleteElem(prev, pos);
                if(*pos != NULL) elem = (tree*) list_Element(*pos);
            }
            if(*pos == NULL) {
                /* Empty conjunction into true */
                p->op = TRUE;
                p->children = NULL;
                *pos = *prev = NULL;
                break;
            } 
            else if(*prev == NULL) {
                if((*pos)->next == NULL){
                    /* Remove unary conjunction */
                    p->op = elem->op;
                    if (tree_Id(elem)) p->id = strdup(elem->id);
                    p->children = elem->children;
                    elem->children = NULL;
                    list_Delete(pos);
                    *pos = *prev = NULL;
                }
                else p->children = *pos;
            }
            else p->children = *prev;
            break;
        case NOT:
            tree_ReplaceParent(FALSE, p, prev, pos);
            break;
        case ALWAYS:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case NEXT:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case SOMETIME:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case OR:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case UNLESS:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case IMPLICATION:
            if(*prev == NULL){
                /* true -> phi = phi */
                elem = (tree*) (*pos)->next->element;
                p->children = NULL;
                (*pos)->next->element = NULL;
                list_Delete(&p->children);
                list_Delete(pos);
                p->op = elem->op;
                if(tree_Id(elem)) p->id = strdup(elem->id);
                p->children = elem->children;
                elem->children = NULL;
                tree_Delete(&elem);
                *pos = NULL;
            }
            else{
                /* phi -> true = true */
                tree_ReplaceParent(op, p, prev, pos);
            }
            break;
        case EQUIVALENCE:
            if(*prev == NULL){
                /* true <-> phi = phi */
                elem = (tree*) (*pos)->next->element;
                p->children = NULL;
                (*pos)->next->element = NULL;
                list_Delete(&p->children);
                list_Delete(pos);
                p->op = elem->op;
                if(tree_Id(elem)) p->id = strdup(elem->id);
                p->children = elem->children;
                elem->children = NULL;
                tree_Delete(&elem);
                *pos = NULL;
            }
            else{
                /* phi <-> true = phi */
                elem = (tree*) (*prev)->element;
                p->children = NULL;
                (*prev)->element = NULL;
                list_Delete(&p->children);
                list_Delete(prev);
                p->op = elem->op;
                if(tree_Id(elem)) p->id = strdup(elem->id);
                p->children = elem->children;
                elem->children = NULL;
                tree_Delete(&elem);
                *prev = *pos = NULL;
            }
            break;
        case UNTIL:
            if(*prev == NULL){
                /* true until phi = sometime phi */
                p->op = SOMETIME;
                p->children = (*pos)->next;
                (*pos)->next = NULL;
                list_Delete(pos);
            }
            else {
                /* phi until true = true */
                tree_ReplaceParent(op, p, prev, pos);
            }
            break;
    }
    if(p != NULL && p->parent != NULL){
        mergesort(&p->parent->children);
    }
}

void falseSimplification(tree **formula, list **prev,  list **pos){
    if(*formula == NULL) return;
    if((*formula)->parent == NULL) {
        simplified = 0;
        return;
    }

    tree *p = (*formula)->parent;
    tree *elem = NULL;
    int p_op = p->op;
    int op = (*formula)->op;
    switch(p_op){
        case OR:
            /* Remove false node from tree */
            elem = *formula;
            while(elem->op == op && !list_IsEmpty(*pos)){
                list_DeleteElem(prev, pos);
                if(*pos != NULL) elem = (tree*) list_Element(*pos);
            }
            if(*pos == NULL) {
                /* Empty disjunction into false */
                p->op = FALSE;
                p->children = NULL;
                *pos = *prev = NULL;
                break;
            } 
            else if(*prev == NULL) {
                if((*pos)->next == NULL){
                    /* Remove unary disjunction */
                    p->op = elem->op;
                    if (tree_Id(elem)) p->id = strdup(elem->id);
                    p->children = elem->children;
                    elem->children = NULL;
                    tree_Delete(&elem);
                    *pos = *prev = NULL;
                    if(p->parent != NULL){
                        mergesort(&p->parent->children);
                    }
                }
                else p->children = *pos;
            }
            else p->children = *prev;
            break;
        case NOT:
            tree_ReplaceParent(TRUE, p, prev, pos);
            break;
        case ALWAYS:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case NEXT:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case SOMETIME:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case AND:
            tree_ReplaceParent(op, p, prev, pos);
            break;
        case IMPLICATION:
            if(*prev == NULL){
                /* false -> phi = true */
                tree_ReplaceParent(TRUE, p, prev, pos);
            }
            else{
                /* phi -> false = not phi */
                p->op = NOT;
                *pos = *prev;
                list_Delete(&(*pos)->next);
                p->children = *pos;
                *prev = NULL;
            }
        case EQUIVALENCE:
            if(*prev == NULL){
                /* false <-> phi = not phi */
                p->op = NOT;
                p->children = (*pos)->next;
                (*pos)->next = NULL;
                list_Delete(pos);
                *pos = p->children;
            }
            else{
                /* phi <-> false = not phi */
                p->op = NOT;
                *pos = *prev;
                list_Delete(&(*pos)->next);
                p->children = *pos;
                *prev = NULL;
            }
            break;
        case UNTIL:
            if(*prev == NULL){
                /* false until phi = phi */
                elem = (tree*) (*pos)->next->element;
                p->children = NULL;
                (*pos)->next->element = NULL;
                list_Delete(&p->children);
                list_Delete(pos);
                p->op = elem->op;
                if(tree_Id(elem)) p->id = strdup(elem->id);
                p->children = elem->children;
                elem->children = NULL;
                tree_Delete(&elem);
                *pos = NULL;
            }
            else{
                /* phi until false = false */
                tree_ReplaceParent(op, p, prev, pos);
            }
            break;
        case UNLESS:
            if(*prev == NULL){
                /* false unless phi = phi */
                elem = (tree*) (*pos)->next->element;
                p->children = NULL;
                (*pos)->next->element = NULL;
                list_Delete(&p->children);
                list_Delete(pos);
                p->op = elem->op;
                if(tree_Id(elem)) p->id = strdup(elem->id);
                p->children = elem->children;
                elem->children = NULL;
                tree_Delete(&elem);
                *pos = NULL;
            }
            else{
                /* phi unless false = always phi */
                p->op = ALWAYS;
                *pos = *prev;
                list_Delete(&(*pos)->next);
                p->children = *pos;
                *prev = NULL;
            }
            break;
    }
    if(p != NULL && p->parent != NULL){
        mergesort(&p->parent->children);
    }
}

void sometimeSimplification(tree **formula, list ** pos){
    if(*formula == NULL) return;

    list *children = (*formula)->children;
    if(children == NULL) return;

    tree *elem = (tree*) children->element;
    list *grandchild = NULL;
    int op = elem->op;

    switch(op){
        case ALWAYS:
            grandchild = elem->children;
            if(grandchild != NULL && 
                    ((tree*)grandchild->element)->op == SOMETIME){
                /* sometime always sometime phi = always sometime phi */
                simplified = 1;
                (*formula)->op = ALWAYS;
                (*formula)->children = elem->children;
                elem->children = NULL;
                list_Delete(&children);
            }
            break;
        case SOMETIME:
            /* sometime sometime phi = sometime phi */
            simplified = 1;
            (*formula)->children = elem->children;
            elem->children = NULL;
            list_Delete(&children);
            break;
    }

}

void alwaysSimplification(tree **formula, list ** pos){
    if(*formula == NULL) return;

    list *children = (*formula)->children;
    if(children == NULL) return;

    tree *elem = (tree*) children->element;
    list *grandchild = NULL;
    int op = elem->op;

    switch(op){
        case ALWAYS:
            /* always always phi = always phi */
            simplified = 1;
            (*formula)->children = elem->children;
            elem->children = NULL;
            list_Delete(&children);
            break;
        case SOMETIME:
            grandchild = elem->children;
            if(grandchild != NULL && 
                    ((tree*)grandchild->element)->op == ALWAYS){
                /* always sometime always phi = sometime always phi */
                (*formula)->op = SOMETIME;
                (*formula)->children = elem->children;
                elem->children = NULL;
                list_Delete(&children);
            }
            break;
    }
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

void findRepeated(int op, tree *parent, list *children){
    if(children == NULL) return;
    if(children->next == NULL) return;

    tree *curr = NULL;
    tree *next = NULL;

    int negation;

    for(list *it = children; it->next != NULL; it = it->next){
        curr = (tree*) list_Element(it);
        next = (tree*) list_Element(it->next);
        negation = 0;

        if(curr->op == NOT){
            if(next->op != NOT){
                /* check if ~curr = next*/
                curr = (tree*) list_Element(curr->children);
                negation = 1;
            }
            else{
                /* eliminate the NOT */
                curr = (tree*) list_Element(curr->children);
                next = (tree*) list_Element(next->children);
            }
        }
        else if(next->op == NOT){
            /*check if curr = ~next*/
            next = (tree*) list_Element(next->children);
            negation = 1;
        }

        if(formula_Compare(curr, next) == 0){
            simplified = 1;
            /* Check the operator to decide what to do with the repeated node */
            switch(op){
                case AND:
                    if(negation){
                        /* Replace parent by false because phi & ~phi*/
                        parent->op = FALSE;
                        list_Delete(&(parent->children));
                        it = NULL;
                        break;
                    }
                case OR:
                    if(negation){
                        /* Replace parent by true because phi | ~phi*/
                        parent->op = TRUE;
                        list_Delete(&(parent->children));
                        it = NULL;
                        break;
                    }
                    /* Remove repeated node from tree whether phi & phi or 
                     * phi | phi*/
                    list *tmp = it->next;
                    it->next = tmp->next;
                    tmp->next = NULL;
                    list_Delete(&tmp);
                    break;
                case IMPLICATION:
                    if(negation) break; /* Nothing to be done for */
                case EQUIVALENCE:
                    if(negation){
                        /* Replace parent by false because phi <-> ~phi */
                        parent->op = FALSE;
                        list_Delete(&(parent->children));
                        it = NULL;
                        break;
                    }
                    /*Replace parent by true whether phi -> phi or phi <-> phi*/
                    parent->op = TRUE;
                    list_Delete(&(parent->children));
                    it = NULL;
                    break;
                case UNTIL:
                    if(negation){
                        /* Replace ~phi U phi by sometime phi */
                        /* Replace phi U ~phi by sometime ~ phi */ 
                        parent->op = SOMETIME;
                        parent->children = it->next;
                        it->next = NULL;
                        list_Delete(&it);
                        break;
                    }
                case UNLESS:
                    if(negation){
                        /* Replace parent by true wheter (phi W ~phi) or 
                         * (~phi W phi) */
                        parent->op = TRUE;
                        list_Delete(&(parent->children));
                        it = NULL;
                        break;
                    }
                    /* Replace parent by phi wheter (phi U phi) or 
                     * (phi W phi) */
                    parent->op = curr->op;
                    parent->children = curr->children;
                    curr->children = NULL;
                    if(tree_Id(curr)) parent->id = strdup(curr->id);
                    tree_Delete(&curr);
                    tree_Delete(&next);
                    it = NULL;
                    break;
                default:
                    printf("Unexpected operator. Something went wrong.\n");
                    abort();
            }
            if(parent != NULL && parent->parent != NULL){
                mergesort(&parent->parent->children);
            }
        }
        if(it == NULL || it->next == NULL) break;
    }
}

void checkUnarity(tree *formula){
    if(formula == NULL) return;
    
    list *it = formula->children;

    if(it->next == NULL){
        simplified = 1;
        tree *elem = (tree*) list_Element(it);
        formula->op = elem->op;
        formula->children = elem->children;
        elem->children = NULL;
        if(tree_Id(elem)) {formula->id = elem->id; elem->id = NULL;}
        list_Delete(&it);
        if(formula->parent != NULL){
            mergesort(&formula->parent->children);
        }
    }
}
