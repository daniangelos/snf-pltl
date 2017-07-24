#ifndef SIMPL_H
#define SIMPL_H 

#include "helpers.h"
#include "parser.tab.h"

#define SIMPFIRSTRULE 46

int simplrules[45];
int simplified;

void initializeRules();
void simplification(list *formulae);
void simplStep(tree **formula, list **prev, list **pos);
void truthParenting(tree **formula, list **prev, list **pos);
void falseParenting(tree **formula, list **prev, list **pos);
int checkEquality(list *subf1, list *subf2);
void findRepeated(int op, list *children);
void checkUnarity(tree *formula);

#endif

