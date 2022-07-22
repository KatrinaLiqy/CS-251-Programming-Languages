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


//(if test trueExpr falseExpr), where the evaluation 
//of trueExpr is returned if test holds (evaluates to 
//a truthy value), else the evaluation of falseExpr is returned
//we parse the args so that arg1 is test, arg2 is trueExpr, arg3 is falseExpr
Value *evalIf(Value *args, Frame *frame){
    Value *curr = args;
    for (int i = 0; i < 3; i++) {
        if (curr->type == NULL_TYPE){
            evaluationError(0,NULL);
        }
        curr = curr->c.cdr;

    }
    if(curr->type != NULL_TYPE){
        evaluationError(1,NULL);
    }

    curr = args;
    curr = curr->c.cdr;

    Value *arg1 = eval(args->c.car, frame); //might not be a BOOL_TYPE

    if(arg1->type != BOOL_TYPE){
        arg1 = eval(arg1,frame);
    }

    if (arg1->i == 1){
        Value *arg2 = eval(curr->c.car, frame); // if there is error in arg2, eval() finds it for us
        return arg2;
    }

    
    curr = curr->c.cdr;
    
    Value *arg3 = eval(curr->c.car, frame);
    return arg3;
    
}