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
#include "built_in.h"
#include "eval_cond.h"
#include "eval_and.h"
#include "eval_or.h"
#include "eval_begin.h"
#include "eval_set.h"


void interpret(Value *tree){
    Frame *globalFrame = talloc(sizeof(Frame));
    globalFrame->parent = NULL;
    globalFrame->bindings = makeNull(); 
    
    bindAllPrimitiveFunction(globalFrame);

    Value *curr = tree; 
    Value *final_result;
    Frame *curr_frame = globalFrame;
    while (curr->type != NULL_TYPE){ 
        
        final_result = eval(car(curr), curr_frame);      
        switch (final_result->type) {
            case NULL_TYPE:
                break;
            case INT_TYPE:
                printf("%d", final_result->i);
                break;
            case DOUBLE_TYPE:
                printf("%lf", final_result->d);
                break;
            case STR_TYPE:
                printf("%c",'"');
                printf("%s",final_result->s);
                printf("%c",'"');
                break;
            case OPEN_TYPE:
                break;
            case CLOSE_TYPE:
                break;
            case BOOL_TYPE:
                if (final_result->i == 0){
                    printf("#f");
                } else {
                    printf("#t");
                }
                break;
            case SYMBOL_TYPE:
                printf("%s",final_result->s);
                break;
            case PTR_TYPE:
                break;
            case CONS_TYPE:
                printTreeAdvanced(final_result);
                break;

            case OPENBRACKET_TYPE:
                break;
            case CLOSEBRACKET_TYPE:
                break;
            case DOT_TYPE:
                break;
            case SINGLEQUOTE_TYPE:
                break;
            case VOID_TYPE:
                curr = curr->c.cdr;
                continue;
            case CLOSURE_TYPE:
                printf("#<procedure>");
                break;
            case PRIMITIVE_TYPE:
                break;
            case UNSPECIFIED_TYPE:
                break;
        }
        curr = curr->c.cdr;
        printf("\n");
    }
}

Value *eval(Value *expr, Frame *frame){
    if (expr->type == INT_TYPE || expr->type == DOUBLE_TYPE || expr->type == BOOL_TYPE || expr->type == STR_TYPE){
        return expr;
    } else if (expr->type == SYMBOL_TYPE){
        Value *result =  lookUpSymbol(expr, frame);
        return result;
    } else if (expr->type == CONS_TYPE){ 
        Value *first = car(expr);
        Value *args = cdr(expr);
        Value *result;
        
        if (strcmp(first->s, "if") == 0) {
            result = evalIf(args, frame);
        } else if (strcmp(first->s, "let") == 0) { 
            result = evalLet(args, frame);
        } else if (strcmp(first->s, "let*") == 0) { 
            result = evalLetAsterisk(args, frame);
        } else if (strcmp(first->s, "letrec") == 0) { 
            result = evalLetrec(args, frame);
        } else if (strcmp(first->s, "quote") == 0) {
            result = evalQuote(args);
        } else if (strcmp(first->s, "define") == 0) { 
            result = evalDef(args, frame);
        } else if (strcmp(first->s, "cond") == 0) { 
            result = evalCond(args, frame);
        } else if (strcmp(first->s, "and") == 0) { 
            result = evalAnd(args, frame);
        } else if (strcmp(first->s, "or") == 0) {
            result = evalOr(args, frame);
        } else if (strcmp(first->s, "begin") == 0) {
            result = evalBegin(args, frame);
        } else if (strcmp(first->s, "set!") == 0) {
            result = evalSet(args, frame);
        } else if (strcmp(first->s, "lambda") == 0) { 
            if (args->type == NULL_TYPE){
                evaluationError(15, NULL);
            }
            result = evalLambda(args, frame);
        } else {
            Value *evaluatedOperator = eval(first, frame);
            Value *evaluatedArgs = evalEach(args, frame); 
            return apply(evaluatedOperator, evaluatedArgs);
            
        }
        return result;
    }
    return expr;
     

}
