#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "eval_let.h"
#include "look_up_symbol.h"
#include "eval_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


//evaluate a let expression
Value *evalLet(Value *args, struct Frame *frame){

    Value *binding = args->c.car; 
    Value *body = args->c.cdr; 
    Value *variable_list = makeNull(); //prevents variable duplication
    if (body->type == NULL_TYPE){
        evaluationError(7, NULL);
    }
    
    Value *curr_binding = binding; 
    Value *bindings = makeNull();

    if(curr_binding->type!= CONS_TYPE){
        evaluationError(2,NULL);
    }
    
    if (curr_binding->c.car->type != NULL_TYPE){

        if (curr_binding->c.car->type!= CONS_TYPE){
            evaluationError(2,NULL);
        }
        
        if (curr_binding->c.car->c.cdr->type == NULL_TYPE){
            evaluationError(6,NULL);
        }

        
        while (curr_binding->type != NULL_TYPE){

            //pointer curr_token is declared to check if there are
            //two elements in each curr_binding
            //so a curr_binding like (), (x) or (x 1 2)
            //should cause evaluationError
            Value *curr_token = curr_binding->c.car;
            
            if (curr_token == NULL){
                evaluationError(2,NULL);
            }
            for (int i = 0; i < 2; i++) {
                if (curr_token == NULL || curr_token->type == NULL_TYPE){
                    evaluationError(2, NULL);
                }
                curr_token = curr_token->c.cdr;
            }
            if(curr_token->type != NULL_TYPE){
                evaluationError(4, NULL);
            }

            //at this point we know curr_binding is in the form of (var expr)
            Value *curr_variable = curr_binding->c.car->c.car; 
            if (curr_variable->type != SYMBOL_TYPE){
                evaluationError(3,NULL);
            }
            Value *existing_variable = variable_list;
            while(existing_variable->type != NULL_TYPE){
                if (strcmp(existing_variable->c.car->s, curr_variable->s) == 0) {
                    evaluationError(8,NULL);
                } 
                existing_variable = existing_variable->c.cdr;
            }
            
            variable_list = cons(curr_variable,variable_list);
            
            Value *curr_value = curr_binding->c.car->c.cdr->c.car; 
                
            
            if (curr_value->type == CONS_TYPE){
                curr_value = eval(curr_value, frame);
            }
            if (curr_value->type == SYMBOL_TYPE){
                curr_value = lookUpSymbol(curr_value, frame);
            }
            
            Value *variable_cell = talloc(sizeof(Value));
            variable_cell->type = CONS_TYPE;
            variable_cell->c.car = curr_variable;
            variable_cell->c.cdr = curr_value;

            bindings = cons(variable_cell, bindings);
            
            curr_binding = curr_binding->c.cdr;
        }
    }
    
    //create a frame
    Frame *let_frame = talloc(sizeof(Frame));
    let_frame->parent = frame;
    let_frame->bindings = bindings;

    Value *curr_body = body;
    Value *last_result;
    
    while(curr_body->type != NULL_TYPE){
        last_result = eval(curr_body->c.car, let_frame);
        curr_body = curr_body->c.cdr;
    }
    
    return last_result;
}

// Evaluates expressions from left to right and then
// the body. The expressions on the right are within
// the scope of the expressions on the left
Value *evalLetAsterisk(Value *args, struct Frame *frame){
    
    Value *binding = args->c.car; 
    
    Value *body = args->c.cdr; 
    if (body->type == NULL_TYPE){
        evaluationError(7, NULL);
    }
    
    Value *curr_binding = binding; 
    Frame *curr_frame = frame;
    

    if(curr_binding->type!= CONS_TYPE){
        evaluationError(2,NULL);
    }
    
    if (curr_binding->c.car->type != NULL_TYPE){

        if (curr_binding->c.car->type!= CONS_TYPE){
            evaluationError(2,NULL);
        }
        
        if (curr_binding->c.car->c.cdr->type == NULL_TYPE){
            evaluationError(6,NULL);
        }

        
        while (curr_binding->type != NULL_TYPE){
            Value *curr_token = curr_binding->c.car;
            if (curr_token == NULL){
                evaluationError(2,NULL);
            }
            for (int i = 0; i < 2; i++) {
                if (curr_token == NULL || curr_token->type == NULL_TYPE){
                    evaluationError(2, NULL);
                }
                curr_token = curr_token->c.cdr;
            }
            if(curr_token->type != NULL_TYPE){
                evaluationError(4, NULL);
            }

            Value *curr_variable = curr_binding->c.car->c.car; 
            if (curr_variable->type != SYMBOL_TYPE){
                evaluationError(3,NULL);
            }
            Value *curr_value = curr_binding->c.car->c.cdr->c.car; 
                
            if (curr_value->type == CONS_TYPE){                
                curr_value = eval(curr_value, curr_frame);
            }
            if (curr_value->type == SYMBOL_TYPE){
                curr_value = lookUpSymbol(curr_value, curr_frame);
            }
            
            Value *variable_cell = talloc(sizeof(Value));
            variable_cell->type = CONS_TYPE;
            variable_cell->c.car = curr_variable;
            variable_cell->c.cdr = curr_value;
            Value *bindings = makeNull();
            bindings = cons(variable_cell, bindings);
            Frame *let_frame = talloc(sizeof(Frame));
            let_frame->parent = curr_frame;
            let_frame->bindings = bindings;
            curr_binding = curr_binding->c.cdr;
            curr_frame = let_frame;
        
        }
    }
    
    Value *curr_body = body;
    Value *last_result;
    
    while(curr_body->type != NULL_TYPE){
        last_result = eval(curr_body->c.car, curr_frame);
        curr_body = curr_body->c.cdr;
    }
    
    return last_result;
}

// Evaluates expressions in unspecified order and
// returns the result of the body.
Value *evalLetrec(Value *args, struct Frame *frame){

    //[x, ] ->[y, ] ->[z ] step 2 x is unspecified type
    //x
    //[evaluated e-1, e-2, e-3] step 3 frame (env-2)
    //[x, e-1]->[y,e-2]...step 4
    //

    Frame *env_2 = talloc(sizeof(Frame));
    env_2->parent = frame;

    Value *binding = args->c.car;
    
    Value *body = args->c.cdr;
    
    Value *variable_list = makeNull(); //prevents variable duplication
    if (body->type == NULL_TYPE){
        evaluationError(7, NULL);
    }
    
    Value *curr_binding = binding; 
    Value *bindings = makeNull();

    if(curr_binding->type!= CONS_TYPE){
        evaluationError(2,NULL);
    }
    if (curr_binding->c.car->type != NULL_TYPE){
        if (curr_binding->c.car->type!= CONS_TYPE){
            evaluationError(2,NULL);
        }
        if (curr_binding->c.car->c.cdr->type == NULL_TYPE){
            evaluationError(6,NULL);
        }

        
        while (curr_binding->type != NULL_TYPE){
            Value *curr_token = curr_binding->c.car;
            if (curr_token == NULL){
                evaluationError(2,NULL);
            }
            for (int i = 0; i < 2; i++) {
                if (curr_token == NULL || curr_token->type == NULL_TYPE){
                    evaluationError(2, NULL);
                }
                curr_token = curr_token->c.cdr;
            }
            if(curr_token->type != NULL_TYPE){
                evaluationError(4, NULL);
            }

            //at this point we know curr_binding is in the form of (var expr)
            Value *curr_variable = curr_binding->c.car->c.car; 
            if (curr_variable->type != SYMBOL_TYPE){
                evaluationError(3,NULL);
            }
            Value *existing_variable = variable_list;
            while(existing_variable->type != NULL_TYPE){
                if (strcmp(existing_variable->c.car->s, curr_variable->s) == 0) {
                    evaluationError(8,NULL);
                } 
                existing_variable = existing_variable->c.cdr;
            }
            
            variable_list = cons(curr_variable,variable_list);

            Value *variable_cell = talloc(sizeof(Value));
            variable_cell->type = CONS_TYPE;
            variable_cell->c.car = curr_variable;
            variable_cell->c.cdr = talloc(sizeof(Value));
            variable_cell->c.cdr->type = UNSPECIFIED_TYPE;

            bindings = cons(variable_cell, bindings);
            
            curr_binding = curr_binding->c.cdr;
        }
    }
    
    bindings = reverse(bindings);
    
    env_2->bindings = bindings;
    //start of step 3
    curr_binding = binding;
    
    while (curr_binding->type != NULL_TYPE){
    
        Value *curr_value = curr_binding->c.car->c.cdr->c.car;
        curr_value = eval(curr_value,env_2);
        curr_binding = curr_binding->c.cdr;
    }
    //end of step 3


    //step 4: implementing [x,1]->[y,2]
    curr_binding = binding;
    Value *curr_cell = bindings;
    while (curr_binding->type != NULL_TYPE){
        curr_cell->c.car->c.car->type = curr_binding->c.car->c.car->type;
        Value *curr_value = curr_binding->c.car->c.cdr->c.car;
        curr_value = eval(curr_value,env_2);
        curr_cell->c.car->c.cdr = curr_value;
        curr_cell = curr_cell->c.cdr;
        curr_binding = curr_binding->c.cdr;
    }
    //create a frame
    Frame *let_frame = talloc(sizeof(Frame));
    let_frame->parent = frame;
    let_frame->bindings = bindings;

    Value *curr_body = body;
    Value *last_result;
    
    //eval the body
    while(curr_body->type != NULL_TYPE){
        last_result = eval(curr_body->c.car, let_frame);
        curr_body = curr_body->c.cdr;
    }
    
    return last_result;
}