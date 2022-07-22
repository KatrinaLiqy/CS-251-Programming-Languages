#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "eval_if.h"
#include "eval_error.h"
#include "eval_def.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// modifies the current active frame. It creates a new binding 
// of the variable to the corresponding expression
Value *evalDef(Value *args, Frame *frame){ 
    if(args->type == NULL_TYPE){
        evaluationError(13,NULL);
    }
    Value *var = args->c.car; 
    if(var->type != SYMBOL_TYPE){
        evaluationError(12,NULL);
    }
    if(args->c.cdr->type == NULL_TYPE){
        evaluationError(14,NULL);
    }
    Value *expr = args->c.cdr->c.car;

    Value *curr_bindings = frame->bindings;
    Value *expr_value = eval(expr,frame);
    Value *ret = talloc(sizeof(Value));
    ret->type = VOID_TYPE;

    if (curr_bindings!= NULL){
        while(curr_bindings->type!= NULL_TYPE){
            if (strcmp(curr_bindings->c.car->c.car->s, var->s) == 0) {
                curr_bindings->c.car->c.cdr = expr_value;
                return ret;
            } else {
                curr_bindings = curr_bindings->c.cdr;
                if(curr_bindings == NULL){
                    printf("NULL!\n");
                }
            }
        }
    }
    Value *variable_cell = talloc(sizeof(Value));
    variable_cell->type = CONS_TYPE;
    variable_cell->c.car = var;
    variable_cell->c.cdr = expr_value;
    frame->bindings = cons(variable_cell, frame->bindings);
    return ret;
}