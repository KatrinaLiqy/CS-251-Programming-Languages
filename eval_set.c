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


// Modifies the value of an already existing variable
Value *evalSet(Value *args, Frame *frame){ 
    
    char *variable = args->c.car->s;
    Value *value = eval(car(cdr(args)), frame);
    Value *curr_bindings = frame->bindings;
    Frame *curr_frame = frame;
    Value *ret = talloc(sizeof(Value));
    ret->type = VOID_TYPE;

    while (curr_frame != NULL){
        curr_bindings = curr_frame->bindings;
            while(curr_bindings->type!= NULL_TYPE){
            
                if (strcmp(curr_bindings->c.car->c.car->s, variable) == 0) {
                    curr_bindings->c.car->c.cdr = value;
                    return ret;
                } else {
                    curr_bindings = curr_bindings->c.cdr;
                }
            }
        curr_frame = curr_frame->parent;
    }
    return ret;
}
