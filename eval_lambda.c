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
#include "value.h"

Value *evalLambda(Value *args, Frame *frame){ 
    Value *closure = talloc(sizeof(Value));
    closure->type = CLOSURE_TYPE;
    Value *params = car(args); 
    Value *body = cdr(args);

    if (body->type == NULL_TYPE){
        evaluationError(16, NULL);
    }
    
    Value *listOfParams = makeNull();
    Value *curr = params;
    while (curr->type != NULL_TYPE){
        if (curr->c.car->type == NULL_TYPE || curr->c.car->type == SYMBOL_TYPE){
            Value *new_params = car(curr);
            if (curr->c.car->type == SYMBOL_TYPE){
                Value *existing_variable = listOfParams;
                while(existing_variable->type != NULL_TYPE){
                    if (strcmp(existing_variable->c.car->s, curr->c.car->s) == 0) {
                        evaluationError(17,NULL);
                    } 
                    existing_variable = existing_variable->c.cdr;
                }
            }
            listOfParams = cons(new_params, listOfParams);

        } else {
            evaluationError(18, NULL);
        }
        curr = cdr(curr);
        
    }  

    Value *reversed = makeNull();
    while (listOfParams->type != NULL_TYPE){
        reversed = cons(car(listOfParams), reversed);
        listOfParams = cdr(listOfParams);
    }
   
    
    closure->cl.paramNames = reversed; 
    closure->cl.functionCode = car(body);
    closure->cl.frame = frame;    

    return closure;

}