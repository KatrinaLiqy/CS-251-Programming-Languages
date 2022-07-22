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

// evaluates each expression in args
Value *evalEach(Value *args, Frame *frame){
    Value *eval_params = makeNull();
    Value *curr = args;
    if (args->type == NULL_TYPE){
        return args;
    }

    while (curr->type != NULL_TYPE){
        Value *evaluated_param = talloc(sizeof(Value));
        evaluated_param = eval(car(curr), frame); 
        eval_params = cons(evaluated_param, eval_params);
        curr = cdr(curr);
    }
    
    Value *reversed = makeNull();
    while (eval_params->type != NULL_TYPE){
        reversed = cons(car(eval_params), reversed);
        eval_params = cdr(eval_params);
    }
    return reversed;

}
