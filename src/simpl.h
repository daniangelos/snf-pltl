#ifndef SIMPL_H
#define SIMPL_H 

#include "helpers.h"
#include "parser.tab.h"
#include "ruletracker.h"

#define SIMPFIRSTRULE 46

int simplified;

void simplification(list *formulae);
void simplStep(tree **formula, list **prev, list **pos);
void truthSimplification(tree **formula, list **prev, list **pos);
void falseSimplification(tree **formula, list **prev, list **pos);
void sometimeSimplification(tree **formula, list **prev, list **pos);
void alwaysSimplification(tree **formula, list **prev, list **pos);
int checkEquality(list *subf1, list *subf2);
void findRepeated(int op, tree *parent, list *children);
void checkUnarity(tree *formula);

#endif

