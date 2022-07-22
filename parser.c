#include "parser.h"
#include "value.h"
#include <ctype.h>
#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// prints either too few or too many close parens error
void syntaxError(int i) {
    if (i == 0){
        printf("Syntax error: too few close parens\n");
    } else if (i == 1){
        printf("Syntax error: too many close parens\n");
    }
    texit(0);
}

// Return a pointer to a parse tree representing the structure of a Scheme 
// program, given a list of tokens in the program.
Value *parse(Value *tokens) {

    Value *current = tokens;
    assert(current != NULL && "Error (parse): null pointer");
   
    Value *stack = makeNull();
    //int depth = 0; 

    while (current->type != NULL_TYPE) {
        Value *token = current->c.car;
        //printf("token type: %d\n", token->type);
        
        if (token->type != CLOSE_TYPE){
            // if token is not close paren, just push it to the stack
            stack = cons(token,stack);
        } else {
            // if token is close paren, pop from stack until ( is popped
            Value *linked_list = makeNull();
            if (stack->type == NULL_TYPE){ 
                syntaxError(1);
            }
            // if the top of stack is already (, need to give it a cons cell
            if(stack->c.car->type == OPEN_TYPE){
                linked_list = cons(makeNull(), linked_list);//
            }
            // popping...
            while(stack->c.car->type != OPEN_TYPE){
                //printf("popping...\n");
                Value *popped = stack->c.car;
                linked_list = cons(popped,linked_list);
                if(stack->c.cdr->type != NULL_TYPE){
                    stack = stack->c.cdr;
                } else {
                    // if there is no ( in stack, it means that the current ) does not match anything
                    syntaxError(1);
                }
            }
            stack = stack->c.cdr; //to pop the (
            
            stack = cons(linked_list,stack);
            
        }
        current = current->c.cdr; //"next"
        //printf("top of stack after while: %d\n",stack->c.car->type);
    }

    Value *temp_stack = stack;

    Value *tree = makeNull();
    while(temp_stack->type != NULL_TYPE){
        //printf("top of stack type: %d\n", temp_stack->type);
        if(temp_stack->c.car->type == OPEN_TYPE){
            // if there is still a ( in stack, that's an extra one
            syntaxError(0);
        } else {
            // normally add stuff to the tree...
            tree = cons(temp_stack->c.car,tree);
            temp_stack = temp_stack->c.cdr;
            //printf("added\n");
        }
    }
    //printf("tree: %d\n",tree->c.car->type);
        
    return tree;
}

// Print a parse tree to the screen in a readable fashion. It should look 
// just like Scheme code (use parentheses to mark subtrees).
void printTree(Value *tree) {
    
    Value *curr = tree;
    // Value *curr_branch;
    
    while(curr->type != NULL_TYPE) {
        
        //printf("type: %d",curr->c.car->type);
        if(curr->c.car->type == CONS_TYPE){
            
            printf("(");
            printTree(curr->c.car); //recursion!
            printf(")");
            
        } else {
            switch (curr->c.car->type) {
                    case NULL_TYPE:
                        break;
                    case INT_TYPE:
                        printf("%d", curr->c.car->i);
                        break;
                    case DOUBLE_TYPE:
                        printf("%lf", curr->c.car->d);
                        break;
                    case STR_TYPE:
                        printf("%c",'"');
                        printf("%s",curr->c.car->s);
                        printf("%c",'"');
                        break;
                    case OPEN_TYPE:
                        break;
                    case CLOSE_TYPE:
                        break;
                    case BOOL_TYPE:
                        if (curr->c.car->i == 0){
                            printf("#f");
                        } else {
                            printf("#t");
                        }
                        break;
                    case SYMBOL_TYPE:
                        printf("%s",curr->c.car->s);
                        break;
                    case PTR_TYPE:
                        break;
                    case CONS_TYPE:
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
                        break;
                    case CLOSURE_TYPE:
                        break;
                    case PRIMITIVE_TYPE:
                        break;
                    case UNSPECIFIED_TYPE:
                        break;
            }
            
            
        }
        if(curr->c.cdr->type != NULL_TYPE){
            //if there is a next element, a space is needed for separation
            printf(" ");
        }
        curr = curr->c.cdr; //"next"
    }
    return;
}

// void printTreeAdvancedTerminal(Value *tree){
//     printf("(");
//     printf
// }
/*
void printTreeAdvanced(Value *tree) {
    Value *curr = tree;
    
    printf("(");
    while(curr->type != NULL_TYPE){
        Value *car = curr->c.car;
        Value *cdr = curr->c.cdr;
        //if(car->type == CONS_TYPE){
            switch (car->type){
                case NULL_TYPE:
                    break;
                case INT_TYPE:
                    // printf("here\n");
                    printf("%d", car->i);
                    break;
                case DOUBLE_TYPE:
                    printf("%lf", car->d);
                    break;
                case STR_TYPE:
                    printf("%c",'"');
                    printf("%s",car->s);
                    printf("%c",'"');
                    break;
                case OPEN_TYPE:
                    break;
                case CLOSE_TYPE:
                    break;
                case BOOL_TYPE:
                    if (car->i == 0){
                        printf("#f");
                    } else {
                        printf("#t");
                    }
                    break;
                case SYMBOL_TYPE:
                    printf("%s",car->s);
                    break;
                case PTR_TYPE:
                    break;
                case CONS_TYPE:
                    // printf("recursion call");
                    // if ( == NULL_TYPE){
                        
                    // }
                    // printf("type: %d!\n",cdr->type);
                    // if (cdr->type == NULL_TYPE){
                    //     printf("printTree: \n");
                    //     printTree(car);
                    //     printf("printTree ends\n");
                    // }
                    printTreeAdvanced(car);
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
                    break;
                case CLOSURE_TYPE:
                    break;
                case PRIMITIVE_TYPE:
                    break;
            }
            
            printf(" ");
            if (cdr->type == NULL_TYPE){
                printf(")");
                return;
            }
            if (cdr->type!= CONS_TYPE){
                printf(". ");
                switch (cdr->type){
                    case NULL_TYPE:
                        break;
                    case INT_TYPE:
                        printf("%d", cdr->i);
                        break;
                    case DOUBLE_TYPE:
                        printf("%lf", cdr->d);
                        break;
                    case STR_TYPE:
                        printf("%c",'"');
                        printf("%s",cdr->s);
                        printf("%c",'"');
                        break;
                    case OPEN_TYPE:
                        break;
                    case CLOSE_TYPE:
                        break;
                    case BOOL_TYPE:
                        if (cdr->i == 0){
                            printf("#f");
                        } else {
                            printf("#t");
                        }
                        break;
                    case SYMBOL_TYPE:
                        printf("%s",car->s);
                        break;
                    case PTR_TYPE:
                        break;
                    case CONS_TYPE:
                        // printf("recursion call");
                        printf("?");
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
                        break;
                    case CLOSURE_TYPE:
                        break;
                    case PRIMITIVE_TYPE:
                        break;
                }
                printf("]");
                return;
            }
            
            curr = cdr;
            

            
    }
    
}*/


void printTreeAdvanced(Value *tree) {
    Value *curr = tree;
    // Value *curr_branch;
    printf("(");
    while(curr->type != NULL_TYPE) {
        if(curr->type != CONS_TYPE) {
            printf(". ");
            switch (curr->type) {
                    case NULL_TYPE:
                        break;
                    case INT_TYPE:
                        printf("%d", curr->i);
                        break;
                    case DOUBLE_TYPE:
                        printf("%lf", curr->d);
                        break;
                    case STR_TYPE:
                        printf("%c",'"');
                        printf("%s",curr->s);
                        printf("%c",'"');
                        break;
                    case OPEN_TYPE:
                        break;
                    case CLOSE_TYPE:
                        break;
                    case BOOL_TYPE:
                        if (curr->i == 0){
                            printf("#f");
                        } else {
                            printf("#t");
                        }
                        break;
                    case SYMBOL_TYPE:
                        printf("%s",curr->s);
                        break;
                    case PTR_TYPE:
                        break;
                    case CONS_TYPE:
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
                        break;
                    case CLOSURE_TYPE:
                        break;
                    case PRIMITIVE_TYPE:
                        break;
                    case UNSPECIFIED_TYPE:
                        break;
            }
            printf(")");
            return;
        } else {
            // printf("curr car type: %d",curr->c.car->type);
            switch (curr->c.car->type) {
                    case NULL_TYPE:
                        break;
                    case INT_TYPE:
                        printf("%d", curr->c.car->i);
                        break;
                    case DOUBLE_TYPE:
                        printf("%lf", curr->c.car->d);
                        break;
                    case STR_TYPE:
                        printf("%c",'"');
                        printf("%s",curr->c.car->s);
                        printf("%c",'"');
                        break;
                    case OPEN_TYPE:
                        break;
                    case CLOSE_TYPE:
                        break;
                    case BOOL_TYPE:
                        if (curr->c.car->i == 0){
                            printf("#f");
                        } else {
                            printf("#t");
                        }
                        break;
                    case SYMBOL_TYPE:
                        printf("%s",curr->c.car->s);
                        break;
                    case PTR_TYPE:
                        break;
                    case CONS_TYPE:
                        printTreeAdvanced(curr->c.car);
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
                        break;
                    case CLOSURE_TYPE:
                        break;
                    case PRIMITIVE_TYPE:
                        break;
                    case UNSPECIFIED_TYPE:
                        break;
            }
            // printf("cdr type: %d\n",curr->c.cdr->type);
            if(curr->c.cdr->type != NULL_TYPE){
            //if there is a next element, a space is needed for separation
                printf(" ");
            }
            curr = curr->c.cdr; //"next"
            
        }
    }
    printf(")");
    return;
}


