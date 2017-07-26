#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snf.h"
#include "parser.tab.h"

extern symbol_table *st;

void snf(list *formulae){
    char *newsymbol;
    tree *lefthandside = NULL;
    tree *righthandside = NULL;
    tree *newnode = NULL;
    list *children = NULL;

    /*Create initial clause*/
    for(list *it = formulae; !list_IsEmpty(it); it = list_Tail(it)){
        /* Add new symbol q1 to st */
        newsymbol = st_NewSymbol(st);

        lefthandside = tree_TreeOp(NAME);
        tree_SetId(newsymbol, lefthandside);

        /* Generate righthandise on the form
         * tau ( always ( or ( not q1 , formula ) ) ) */
        righthandside = variant_one(newsymbol, (tree*) list_Element(it));

        children = list_New();
        children->next = list_New();
        children->element = lefthandside;
        children->next->element = righthandside;

        newnode = tree_TreeOp(AND);
        newnode->children = children;
        it->element = newnode;
    }
}

tree* variant_one(char *symbol, tree* formula){
    if(symbol == NULL || formula == NULL) return NULL;

    int op = formula->op;
    int p_op = 0;
    tree *element = NULL;
    tree *snf = NULL;
    tree *p = formula->parent;
    if(p != NULL) p_op = p->op;

    switch(op){
        case NOT:
            element = (tree*) list_Element(formula->children);
            return negated_variant_one(symbol, element);
            break;
        case AND:
            /* Conjunction of each transformation */
            break;
        /*case OR:*/
            /*break;*/
        case IMPLICATION:
            /* tau(phi -> psi) = tau(~phi or psi) */
            formula->op = OR;
            element = (tree*) list_Element(formula->children); // phi
            formula_Negate(&element);
            formula->children->element = element;
            return variant_one(symbol, formula);
            break;
        case EQUIVALENCE:
            /* tau(phi <-> psi) = tau(~phi or psi) and tau(phi or ~psi) */
        case NEXT:
        case ALWAYS:
        case SOMETIME:
        case UNTIL:
        case UNLESS:
        default:
            /* If no other rule applies: tau(phi) = phi */
            snf = formula_SNF(symbol, formula);
            return snf;
            break;
    }
}

tree* negated_variant_one(char *symbol, tree* formula){
    if(symbol == NULL || formula == NULL) return NULL;

    int op = formula->op;
    int p_op = 0;
    tree *element = NULL;
    tree *snf = NULL;
    tree *p = formula->parent;
    if(p != NULL) p_op = p->op;

    switch(op){
        case NOT:
            element = (tree*) list_Element(formula->children);
            return variant_one(symbol, element);
            break;
        case AND:
            /* Transformation of disjunction of negated formulae*/
            break;
        case OR:
        case IMPLICATION:
            /* tau(~(phi -> psi)) = tau(phi) and tau(~psi) */
            return snf;
            break;
        case EQUIVALENCE:
        case NEXT:
        case ALWAYS:
        case SOMETIME:
        case UNTIL:
        case UNLESS:
        default:
            /* If no other rule applies: tau(phi) = phi */
            break;
    }

    return NULL;
}
