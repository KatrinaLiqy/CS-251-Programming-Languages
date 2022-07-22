#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "eval_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
 * eval_error.c contains if statements that are called each time
 * a potential error is encountered
 */
void evaluationError(int i, char* symbol){
    if (i == 0){
        printf("Evaluation error: no consequent following an if.\n");
    } else if(i == 1){
        printf("Evaluation error: too much consequent following an if.\n");
    } else if(i == 2){
        printf("Evaluation error: bad form in let.\n");
    } else if(i == 3){
        printf("Evaluation error: left side of a let pair doesn't have a variable.\n");
    } else if(i == 4){
        printf("Evaluation error: too many values assigned to same symbol.\n");
    } else if(i == 5){
        printf("Evaluation error: Symbol '%s' not found.\n", symbol);
    } else if(i == 6){
        printf("Evaluation error: null binding in let.\n");
    } else if(i == 7){
        printf("Evaluation error: no args following the bindings in let.\n");
    } else if(i == 8){
        printf("Evaluation error: duplicate variable in let.\n");
    } else if (i == 10){
        printf("Evaluation error.\n");
    } else if (i == 11){
        printf("Multiple arguments to quote.\n");
    } else if (i == 12){
        printf("Evaluation error: define must bind to a symbol.\n");
    } else if (i == 13){
        printf("Evaluation error: no args following define.\n");
    } else if (i == 14){
        printf("Evaluation error: no value following the symbol in define.\n");
    } else if (i == 15){
        printf("Evaluation error: no args following lambda.\n");
    } else if (i == 16){
        printf("Evaluation error: no code in lambda following parameters.\n");
    } else if (i == 17){
        printf("Evaluation error: duplicate identifier in lambda.\n");
    } else if (i == 18){
        printf("Evaluation error: formal parameters for lambda must be symbols.\n");
    } else if (i == 19){
        printf("Evaluation error: cons takes two arguments, three or more supplied.\n");
    } else if (i == 20){
        printf("Evaluation error: cons takes two arguments, only one supplied.\n");
    } else if (i == 21){
        printf("Evaluation error: no arguments supplied to cons.\n");
    } else if (i == 22){
        printf("Evaluation error: car takes a pair\n");
    } else if (i == 23){
        printf("Evaluation error: car takes one argument\n");
    } else if (i == 24){
        printf("Evaluation error: no arguments supplied to cdr\n");
    } else if (i== 25){
        printf("Evaluation error: no arguments supplied to null?\n");
    } else if (i== 26){
        printf("Evaluation error: null? takes one argument\n");
    } else if (i == 27){
        printf("Evaluation error: + must take numbers.\n");
    } else if (i == 28){
        printf("Evaluation error: letrec used a variable before all were computed.\n");
    }

    texit(0);
}
