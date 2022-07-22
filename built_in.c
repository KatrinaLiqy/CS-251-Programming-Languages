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

 /* 
 * built_in.c contains the primitive functions as well
 * as the functions that bind the function name to the function itself
 */

Value *builtInNull(Value *args) {
    if (args->type == NULL_TYPE){
        evaluationError(25, NULL);
    }
    if (length(args) > 1){
        evaluationError(26, NULL);
    }
    Value *content = car(args);    
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;

    Value *prev_content = content;
    while (content->type == CONS_TYPE){
        prev_content= content;
        content = car(content);
        }   
    if (content->type == NULL_TYPE){
        result->i = 1;
    } else {
        result->i = 0;
    }
    return result;
}

Value *builtInCar(Value *args) {
    
    if (args->c.car->type != CONS_TYPE){
        evaluationError(22,NULL);
    }
    if (args->c.cdr->type != NULL_TYPE){
        evaluationError(23,NULL);
    }
    return args->c.car->c.car;
}

Value *builtInCdr(Value *args) {
    if(args->type==NULL_TYPE){
        evaluationError(24,NULL);
    }
    return args->c.car->c.cdr;
}

Value *builtInCons(Value *args) {
    if(args->type==NULL_TYPE){
        evaluationError(21,NULL);
    }
    Value *car = args->c.car;
    if(args->c.cdr->type==NULL_TYPE){
        evaluationError(20,NULL);
    }
    Value *cdr = args->c.cdr->c.car;
    if(args->c.cdr->c.cdr->type != NULL_TYPE){
        evaluationError(19,NULL);
    }
    Value *cons = talloc(sizeof(Value));
    cons->type = CONS_TYPE;
    cons->c.car = car;
    cons->c.cdr = cdr;

    return cons;
}

Value *builtInPlus(Value *args) {
    Value *sumTotal = talloc(sizeof(Value));
    sumTotal->type = INT_TYPE;
    sumTotal->i = 0;
    if (args->type == NULL_TYPE){
        return sumTotal;
    }
    
    Value *sumI = talloc(sizeof(Value));
    sumI->type = INT_TYPE;
    sumI->i = 0;
    Value *sumD = talloc(sizeof(Value));
    sumD->type = DOUBLE_TYPE;
    sumD->d = 0.0;
    Value *curr = args;
    

    while (curr->type != NULL_TYPE){

        Value *curr_number = curr->c.car;
        if (curr_number->type == BOOL_TYPE){
            evaluationError(27, NULL);
        } else if (curr_number->type == INT_TYPE){
            sumI->i += curr_number->i;
        } else if (curr_number->type == DOUBLE_TYPE){
            sumD->d += curr_number->d;
        }
        curr = cdr(curr);
    }

    if (sumD->d != 0.0){
        sumTotal->type = DOUBLE_TYPE;
        sumTotal->d = sumD->d + (double)sumI->i;
    } else{
        sumTotal->type = INT_TYPE;
        sumTotal->i = sumI->i;   
    }  
    return sumTotal;
}

Value *builtInMinus(Value *args) {
    Value *diffTotal = talloc(sizeof(Value));
    bool flag = true;

    if (args->type == NULL_TYPE){
        return diffTotal;
    }

    if (args->c.car->type == INT_TYPE){
        diffTotal->type = INT_TYPE;
        bool flag = true;
        diffTotal->i = args->c.car->i;
    } else if (args->c.car->type == DOUBLE_TYPE){
        diffTotal->type = DOUBLE_TYPE;
        bool flag = false;
        diffTotal->d = args->c.car->d;
    }

    args = cdr(args);
    Value *curr = args;

    while (curr->type != NULL_TYPE){
        Value *curr_number = curr->c.car;
        if (curr_number->type == BOOL_TYPE){
            evaluationError(27, NULL);
        }

        if (curr_number->type == DOUBLE_TYPE){
            if (flag == true){
                flag = false;
                diffTotal->d -= diffTotal->i;
            }
            if (diffTotal->type != DOUBLE_TYPE){
                diffTotal->type = DOUBLE_TYPE;
            }
            diffTotal->d -= curr->c.car->d;
        }  else if (curr_number->type == INT_TYPE && flag == true){
            diffTotal->i -= curr->c.car->i;
        }
        
        
        else if (curr_number->type == INT_TYPE && flag == false){
            diffTotal->d -= curr->c.car->i;
        }

        curr = cdr(curr);
    }
    return diffTotal;
}

Value *builtInMultiply(Value *args) {
    Value *multTotal = talloc(sizeof(Value));
    multTotal->type = INT_TYPE;
    multTotal->i = 0;
    if (args->type == NULL_TYPE){
        return multTotal;
    }
  
    Value *multI = talloc(sizeof(Value));
    multI->type = INT_TYPE;
    multI->i = 1;
    Value *multD = talloc(sizeof(Value));
    multD->type = DOUBLE_TYPE;
    multD->d = 1.0;
    Value *curr = args;

    while (curr->type != NULL_TYPE){
        

        Value *curr_number = curr->c.car;
        if (curr_number->type == BOOL_TYPE){
            evaluationError(27, NULL);
        } else if (curr_number->type == INT_TYPE){
            multI->i *= curr_number->i;
        } else if (curr_number->type == DOUBLE_TYPE){
            multD->d *= curr_number->d;
        }

        curr = cdr(curr);
    }
        multTotal->type = INT_TYPE;
        multTotal->i = multI->i;  
     
    return multTotal;
}

Value *builtInDivide(Value *args) {
    Value *divTotal = talloc(sizeof(Value));
    bool flag = true;
    Value *divI = talloc(sizeof(Value));
    divI->type = INT_TYPE;
    divI->i = 0;
    Value *divD = talloc(sizeof(Value));
    divD->type = DOUBLE_TYPE;
    divD->d = 0.0;

    if (args->type == NULL_TYPE){
        return divTotal;
    }

    Frame *empty_frame = talloc(sizeof(Frame));
    empty_frame->parent = NULL;
    empty_frame->bindings = makeNull(); 
    bindAllPrimitiveFunction(empty_frame);

    Value *curr_number = eval(args->c.car,empty_frame);
        
    if (curr_number->type == BOOL_TYPE){
        evaluationError(27, NULL);
    } 

    if (args->c.car->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        divTotal->type = DOUBLE_TYPE;
        divTotal->d= (args->c.car->d/car(cdr(args))->i);
    }  else if (args->c.car->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        divTotal->type = DOUBLE_TYPE;
        divTotal->d = (args->c.car->d/car(cdr(args))->d);
        divTotal->type = DOUBLE_TYPE;
    }  else if (args->c.car->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        divTotal->type = DOUBLE_TYPE;
        divTotal->d = (args->c.car->i/car(cdr(args))->d);
        divTotal->type = DOUBLE_TYPE;
    }  else if (args->c.car->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        if ((args->c.car->i%car(cdr(args))->i) == 0){
            divTotal->type = INT_TYPE;
            divTotal->i = ( args->c.car->i/car(cdr(args))->i);
        }else {
            car(cdr(args))->type = DOUBLE_TYPE;
            divTotal->type = DOUBLE_TYPE;
            car(cdr(args))->d = car(cdr(args))->i;
            divTotal->d =  ( args->c.car->i/car(cdr(args))->d);
            
        }
    }

    return divTotal;

}

Value *builtInModulo(Value *args) {
    Value *mod = talloc(sizeof(Value));
  
    if (args->type == NULL_TYPE){
        return mod;
    }
    mod->type = INT_TYPE;
    if (args->c.car->type == INT_TYPE && car(cdr(args))->type == INT_TYPE ){
        mod->i = args->c.car->i % car(cdr(args))->i;
    }
    return mod;
}



Value *builtInLessThan(Value *args) {
    Value *less_than = talloc(sizeof(Value));
    less_than->type = BOOL_TYPE;
    int first_num = args->c.car->i;
    int second_num = args->c.cdr->c.car->i;

    if (first_num < second_num){
        less_than->i = 1;
    } else{
        less_than->i = 0;
    }

    return less_than;
}

Value *builtInGreaterThan(Value *args) {
    Value *greater_than = talloc(sizeof(Value));
    greater_than->type = BOOL_TYPE;
    int first_num = args->c.car->i;
    int second_num = args->c.cdr->c.car->i;

    if (first_num <= second_num){
        greater_than->i = 0;
    }else{
        greater_than->i = 1;
    }

    return greater_than;
}

Value *builtInEqualTo(Value *args) {
    Value *equalTo = talloc(sizeof(Value));
    equalTo->type = BOOL_TYPE;

    int first_num = args->c.car->i;
    int second_num = args->c.cdr->c.car->i;

    if (first_num == second_num){
        equalTo->i = 1;
    }else{
        equalTo->i = 0;
    }

    return equalTo;
}

// binds the primitive type to
// the corresponding c function
void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame) {
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *variable_cell = talloc(sizeof(Value));
    variable_cell->type = CONS_TYPE;
    Value *function_name = talloc(sizeof(Value));
    function_name->type = SYMBOL_TYPE;
    function_name->s = name;
    variable_cell->c.car = function_name;
    variable_cell->c.cdr = value;
    frame->bindings = cons(variable_cell,frame->bindings);
}

// calls all the functions that actually bind the primitive type to
// the corresponding c function
void bindAllPrimitiveFunction(Frame *globalFrame){
    bindPrimitiveFunction("null?", &builtInNull, globalFrame);
    bindPrimitiveFunction("car", &builtInCar, globalFrame);
    bindPrimitiveFunction("cdr", &builtInCdr, globalFrame);
    bindPrimitiveFunction("cons", &builtInCons, globalFrame);
    bindPrimitiveFunction("+", &builtInPlus, globalFrame);
    bindPrimitiveFunction("-", &builtInMinus,globalFrame);
    bindPrimitiveFunction("*", &builtInMultiply, globalFrame);
    bindPrimitiveFunction("/", &builtInDivide, globalFrame);
    bindPrimitiveFunction("modulo", &builtInModulo, globalFrame);
    bindPrimitiveFunction("<", &builtInLessThan, globalFrame);
    bindPrimitiveFunction(">", &builtInGreaterThan, globalFrame);
    bindPrimitiveFunction("=", &builtInEqualTo, globalFrame);
}