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
#include "eval_def.h"
#include "look_up_symbol.h"
#include "eval_quote.h"
#include "eval_lambda.h"
#include "eval_each.h"
#include "apply.h"

// return the result of executing the parameter function on the parameter arguments
Value *apply(Value *function, Value *args){ 
    if(function->type == PRIMITIVE_TYPE){
        
        Value *result =(function->pf)(args);
        return result;
    }
    Value *curr_body = function->cl.functionCode;

    //create a new frame
    Frame *frame = talloc(sizeof(Frame));
    frame->parent = function->cl.frame;

    //add bindings to frame where bindings is a pointer to the head of 
    //a linked list (see figure from assign. 13)
    Value *curr_param = function->cl.paramNames; 
    
    Value *curr_args = args;
    Value *bindings = makeNull();
    if (curr_param->c.car->type != NULL_TYPE){
    
        while (curr_param->type != NULL_TYPE){
            Value *new_binding = talloc(sizeof(Value));
            new_binding->type = CONS_TYPE;
            new_binding->c.car = curr_param->c.car;
            new_binding->c.cdr = curr_args->c.car;      
            bindings = cons(new_binding, bindings);
            curr_param = cdr(curr_param);
            curr_args = cdr(curr_args);
        }
    }
    frame->bindings = bindings; 
    Value *result = eval(function->cl.functionCode, frame);

    return result;
    
}
