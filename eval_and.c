#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "eval_if.h"
#include "eval_error.h"
#include "eval_def.h"
#include "built_in.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "eval_and.h"

//Evaluates both arguments and returns #t if both are true, and #f in all other cases
Value *evalAnd(Value *args, Frame *frame){
    Value *result = talloc(sizeof(Value));
    Value *curr = args;
    while(curr->type != NULL_TYPE){
        result = eval(curr->c.car, frame);
        if (result->i == 0){
            return result;
        }
        curr = cdr(curr);
    }
    return result;
}