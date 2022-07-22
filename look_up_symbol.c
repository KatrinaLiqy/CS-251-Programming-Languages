#include "parser.h"
#include "interpreter.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "look_up_symbol.h"
#include "eval_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


/*
struct Frame {
    struct Frame *parent;
    Value *bindings;
};
*/
Value *lookUpSymbol(Value *variable, Frame *frame){
    //printf("type: %d",variable->type);
    //printf("%s",variable->s);
    // printf("calling lookupsymbol\n");

    Value *curr_bindings = frame->bindings;
    if (curr_bindings!= NULL){
        while(curr_bindings->type!= NULL_TYPE){
            // printf("while looping\n");
            // printf("curr_bindings->c.car->c.car type: %d\n", curr_bindings->c.car->c.car->type);
            // printf("variable: %s\n", curr_bindings->c.car->c.car->s);
            // printf("variable we look for: %s\n",variable->s);
            //if(curr_bindings->c.car->c.car->s == variable->s){
            // printf("variable: %s\n",variable->s);
            if (strcmp(curr_bindings->c.car->c.car->s, variable->s) == 0) {
                if(curr_bindings->c.car->c.cdr->type == UNSPECIFIED_TYPE){
                    //printf("error!");
                    evaluationError(28,NULL);
                }
                // printf("found!\n");
                //printf("value type: %d\n", curr_bindings->c.car->c.cdr->type);
                //printf("value: %d\n",curr_bindings->c.car->c.cdr->i);
                // if (curr_bindings->c.car->c.cdr->type == PRIMITIVE_TYPE){
                //     //double (*ptr)(int, int) = &add;
                //     //double result = (*ptr)(77, 23);
                //     //printf("%f\n", result);
                //     return curr_bindings->c.car->c.cdr->pf;
                // }
                // printf("curr_bindings->c.car->c.cdr type: %d", curr_bindings->c.car->c.cdr->type);
                
                return curr_bindings->c.car->c.cdr;
            } else {
                // printf("next...\n");
                curr_bindings = curr_bindings->c.cdr;
            }
        }
    }
    if (frame->parent == NULL){
        evaluationError(5,variable->s);
    } 
    
    // printf("parent...\n");
    return lookUpSymbol(variable, frame->parent);
    //}
    //return variable;
}