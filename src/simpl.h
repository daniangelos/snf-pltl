#ifndef SIMPL_H
#define SIMPL_H 

#include "helpers.h"
#include "parser.tab.h"

void simplification(list *formulae);
void simplStep(tree *formula);
void truthParenting(tree *formula);
void falseParenting(tree *formula);
int checkEquality(list *subf1, list *subf2);
int isAtomic(tree *formula);
int firstborn(tree *formula);

#endif

