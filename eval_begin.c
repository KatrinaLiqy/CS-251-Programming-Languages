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

//evaluate both arguments and returns the result of the final arguments
Value *evalBegin(Value *args, Frame *frame){
    Value *curr_args = args;
    Value *result;
    while (curr_args->type != NULL_TYPE){
        Value *curr_expr = car(curr_args);
        result = eval(curr_expr, frame);
        curr_args = cdr(curr_args);
    }
    return result;
} 