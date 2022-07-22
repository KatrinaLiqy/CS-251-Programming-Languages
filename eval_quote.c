#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "eval_let.h"
#include "eval_if.h"
#include "eval_error.h"
#include "eval_quote.h"


//returns an unquoted version of the car of the expression
Value *evalQuote(Value *args) {

    Value *curr = args;
    if (curr->type == NULL_TYPE){
        evaluationError(10, NULL);
    }
    if(curr->type != CONS_TYPE){
        printf("?");
    }
    if(curr->c.cdr->type != NULL_TYPE){
        evaluationError(10,NULL);
        curr = curr->c.cdr;
        if(curr->type == CONS_TYPE){
            evaluationError(11, NULL);
        }
        printf("\n");
        texit(0);
    } 
    return args->c.car;
    
}
