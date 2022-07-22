#include <stdio.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "parser.h"
#include "talloc.h"
#include "interpreter.h"

int main() {
    // printf("starting\n");
    Value *tokensList = tokenize();
    // printf("finish tokeninzing");
    Value *tree = parse(tokensList);
    // printf("finish parsing");
    interpret(tree);
    

    tfree();
    return 0;


}
