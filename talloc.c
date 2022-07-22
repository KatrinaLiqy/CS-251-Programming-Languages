#include <stdlib.h>
#include <stdio.h>
#include "value.h"


// Global variable holding the head of the active list (i.e., the linked
// list containing pointers to all of the memory that has been allocated
// via calls to talloc).
Value *g_active_list = NULL; // has PTR_TYPE
// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in a linked list, and you have license here to duplicate code
// that you wrote for linkedlist.c. To be clear, don't actually call functions 
// that are defined in linkedlist.h, because then you'll end up with circular
// dependencies, since you'll be using talloc in linkedlist.c.
void *talloc(size_t size){
    if (g_active_list == NULL) {
        Value *last = malloc(sizeof(Value));
        last->type = NULL_TYPE;
        g_active_list = last;
        //g_active_list->type = CONS_TYPE;
    }

    Value *new_car = malloc(sizeof(Value));
    new_car->type = PTR_TYPE;
    Value *val = malloc(size); //malloc (sizeof(size))?
    new_car->p = val;

    Value *new_cell = malloc(sizeof(Value));
    new_cell->type = CONS_TYPE;
    new_cell->c.car = new_car;
    new_cell->c.cdr = g_active_list;

    g_active_list = new_cell;
    g_active_list->type = CONS_TYPE;

    return val;
}
// Free all pointers allocated by talloc, as well as whatever memory you
// allocated for purposes of maintaining the active list. Hint: consider 
// that talloc may be called again after tfree is called...
void tfree(){
    Value *curr = g_active_list;
    if (curr != NULL){  
        while(curr->type != NULL_TYPE) {
            if (curr->type == CONS_TYPE){
                //printf("hello!!!\n");
                Value *curr_car = curr->c.car;
                Value *curr_cdr = curr->c.cdr;
                Value *curr_ptr = curr->c.car->p;
                // if (curr_ptr->type == STR_TYPE) {
                //     free(curr_ptr->s);
                // }
                free(curr_ptr);
                free(curr_car);
                free(curr);
                curr = curr_cdr;
            } 
        }
        free(curr);
        g_active_list = NULL;
    }
    
}

// void tfree_helper(Value *list){
//     if(list->type == CONS_TYPE){
//         //tfree_helper(list->c.car->p);
//         tfree_helper(list->c.car);
//         tfree_helper(list->c.cdr);
//     } else if(list->type == STR_TYPE){
//         free(list->s);
//     } else if(list->type == PTR_TYPE){
//         tfree_helper(list->p);
//     }
//     free(list);
// }

// Replacement for the C function 'exit' that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on, since you'll be able
// to call it to clean up memory and exit your program whenever an error occurs.
// Briefly look up exit to get a sense of what the 'status' parameter does.
void texit(int status){
    tfree();
    exit(0);
}