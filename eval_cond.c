#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "eval_if.h"
#include "eval_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


// Checks for the first #t value it encounters and returns the corresponding value.
// If no value is true, it has else as a default case.
Value *evalCond(Value *args, Frame *frame){
    Value *curr_bindings = args; 
    while(curr_bindings->type != NULL_TYPE){
        if (curr_bindings->c.car->c.car->type != BOOL_TYPE){
            if (!(strcmp(curr_bindings->c.car->c.car->s, "else"))){
                return eval(curr_bindings->c.car->c.cdr->c.car, frame);
            }
        }
        
        Value *curr_bool = curr_bindings->c.car->c.car;
        curr_bool = eval(curr_bool, frame);
        if(curr_bool->i == 1){
            Value *curr_expr = curr_bindings->c.car->c.cdr->c.car;
            return eval(curr_expr,frame);
        }
        curr_bindings = curr_bindings->c.cdr;
    }
    Value *res = talloc(sizeof(Value));
    res->type = VOID_TYPE;
    return res;
}