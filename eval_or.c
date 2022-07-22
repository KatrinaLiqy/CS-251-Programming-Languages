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
#include "eval_or.h"

//evaluates both arguments and returns #t if even if one is true, and #f when both are #f
Value *evalOr(Value *args, Frame *frame){
    Value *result = talloc(sizeof(Value));
    Value *curr = args;
    while(curr->type != NULL_TYPE){
        result = eval(curr->c.car, frame);
        if (result->i == 1){
            return result;
        }
        
        curr = cdr(curr);
    }
//returns true if at least one of them is true
    return result;
}