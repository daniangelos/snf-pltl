#include <stdlib.h>
#include "helpers.h"

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
