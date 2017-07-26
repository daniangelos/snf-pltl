#ifndef SNF_H
#define SNF_H

#include "helpers.h"

void snf(list *formulae);
tree* variant_one(char *symbol, tree* formula);
tree* negated_variant_one(char *symbol, tree* formula);

#endif //SNF_H
