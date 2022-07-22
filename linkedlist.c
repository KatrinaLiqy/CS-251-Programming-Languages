#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "value.h"
#include <string.h>
#include <assert.h>
#include "talloc.h"

// Create a pointer to a new NULL_TYPE Value
Value *makeNull(){
    Value *head = talloc(sizeof(Value));
    head->type = NULL_TYPE;
    return head;
}


// Return whether the given pointer points at a NULL_TYPE Value.


bool isNull(Value *value){
    if (value->type == NULL_TYPE){
        return true;
    } else {
        return false;
    }
    
}
//  head = cons(val1, head);
// Create a pointer to a new CONS_TYPE Value
Value *cons(Value *newCar, Value *newCdr){
    Value *cons = talloc(sizeof(Value));
    cons->type = CONS_TYPE;
    cons->c.car = newCar;
    cons->c.cdr = newCdr;
    return cons;
}

// Return a pointer to the car value for the cons cell at the head of the given 
// linked list. Use assertions here to make sure that this is a legitimate operation 
Value *car(Value *list){
    return list->c.car;
}

// Return a pointer to the cdr value for the cons cell at the head of the given 
// linked list. Use assertions here to make sure that this is a legitimate operation 
Value *cdr(Value *list){
    return list->c.cdr;
}

// Display the contents of the linked list to the screen in the
// format of a Scheme list -- e.g., ( 33 "lol" 9.9 ). It's okay
// to just use printf here, though you'll have to add the quotes in
// yourself, for strings.
void display(Value *list){
    //printf("(");
    Value *curr = list;
    while (curr->type != NULL_TYPE){
        switch (curr->c.car->type) {
            case INT_TYPE:
                printf("%i", curr->c.car->i);
                break;
            case DOUBLE_TYPE:
                printf("%f", curr->c.car->d);
                break;
            case STR_TYPE:
                printf("%s", curr->c.car->s);
                break;
            case CONS_TYPE:
                //printf("%s", list->c.car->s);
                break;
            case NULL_TYPE:
                //printf("%s", list->c.car->s);
                break;
            case PTR_TYPE:
                break;
            case OPEN_TYPE:
                break;
            case CLOSE_TYPE:
                break;
            case BOOL_TYPE:
                printf("%i", curr->c.car->i);
                break;
            case SYMBOL_TYPE:
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
        curr = curr->c.cdr;
    }
    //printf(")\n");
}
 
// Return a new list that is the reverse of the one that is passed in.
// Value *reverse(Value *list){
//     Value *new_list;
//     Value *curr = list;
//     new_list = makeNull();
//     while (curr->type != NULL_TYPE){
//         Value *new_car = talloc(sizeof(Value));
//         if (curr->c.car->type == STR_TYPE || curr->c.car->type == SYMBOL_TYPE){ // if current car is string
//             char *text = curr->c.car->s;
//             char *new_str = talloc(sizeof(char)*(strlen(text) + 1));
//             new_str = strcpy(new_str,curr->c.car->s);
//             new_car->type = curr->c.car->type;
//             new_car->s = new_str;
//         } else { // if current car is integer or double
//             new_car->type = curr->c.car->type;
//             if(curr->c.car->type == INT_TYPE || curr->c.car->type == BOOL_TYPE){
//                 new_car->i = curr->c.car->i;
//             } else if (curr->c.car->type == DOUBLE_TYPE){
//                 new_car->d = curr->c.car->d;
//             } 
//         }  
//         new_list = cons(new_car, new_list);
//         curr = curr->c.cdr; 
//     }
//     return new_list;
// }

Value *reverse(Value *list){
    Value *new_list;
    Value *curr = list;
    new_list = makeNull();
    while (curr->type != NULL_TYPE){
        new_list = cons(car(curr), new_list);
            curr = cdr(curr);
        }
    
    return new_list;
}


// Return the length of the given list, i.e., the number of cons cells. 
// Use assertions to make sure that this is a legitimate operation.
int length(Value *list){
    int count = 0;
    while (list->type != NULL_TYPE){
        count++;
        list = list->c.cdr;
    }
    return count;
}



/*
int main() {
    return 0
}
*/