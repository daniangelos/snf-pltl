#ifndef CLAUSES_H
#define CLAUSES_H 

#define CINITIAL  0
#define CLITERAL  1
#define CSTEP     2
#define CSOMETIME 3

#include "uthash.h"

struct literalshash{
    int literal;
    UT_hash_handle hh;
};

struct clause{
    int type; // initial, literal, step or sometime
    int size;
    int max_literal;
    int min_literal;

    struct literalshash *right;
    struct literalshash *left;
};

typedef struct clause clause;
typedef struct literalshash literalshash;


#endif //CLAUSES_H
