#include <stdio.h>
#include "value.h"
#include <ctype.h>
#include "linkedlist.h"
#include "talloc.h"



int g_error = 0;

// Read source code that is input via stdin, and return a linked list consisting of the
// tokens in the source code. Each token is represented as a Value struct instance, where
// the Value's type is set to represent the token type, while the Value's actual value
// matches the type of value, if applicable. For instance, an integer token should have
// a Value struct of type INT_TYPE, with an integer value stored in struct variable i.
// See the assignment instructions for more details. 
Value *tokenize() {

    // Prepare list of tokens
    Value *tokensList = makeNull();


    // Prepare the character stream
    char nextChar;
    nextChar = (char)fgetc(stdin);
    
    // Start tokenizing!
    while (nextChar != EOF) {
        int valid = 0;
        Value *newValue = talloc(sizeof(Value));
        if (nextChar == '(') {
            //open
            newValue->type = OPEN_TYPE;        
            valid = 1;

        } else if (nextChar == ')') {
            //close
            newValue->type = CLOSE_TYPE;
            valid = 1;

        } else if (nextChar == '#') {
            //boolean
            nextChar = (char)fgetc(stdin);
            newValue->type = BOOL_TYPE;
            newValue->i = 0;
            if (nextChar == 't' || nextChar == 'f'){
                if(nextChar == 't'){
                    newValue->i = 1;
                } else {
                    newValue->i = 0;
                }
                nextChar = (char)fgetc(stdin);
                if(nextChar == ')'){
                    tokensList = cons(newValue, tokensList);
                    newValue = talloc(sizeof(Value));
                    newValue->type = CLOSE_TYPE;
                } else if (nextChar != ' ' && nextChar != '\n') { 
                    
                    printf("Evaluation error (readBoolean): boolean was not #t or #f\n");
                    g_error = 1;
                    //exit(0);
                }
            } else {
                printf("Evaluation error (readBoolean): boolean was not #t or #f\n");
                g_error = 1;
                //exit(0); 
            }
            valid = 1;

        } else if (isdigit(nextChar) || nextChar == '-') {
            //integer/double/negative value/minus sign
            int isNegative = 0;
            int isSymbol = 0;
            if(nextChar == '-'){
                isNegative = 1;
                nextChar = (char)fgetc(stdin);
                if(nextChar == ' '){
                    newValue->type = SYMBOL_TYPE;
                    newValue->s = "-";
                    isSymbol = 1;
                } else if (nextChar == ')'){
                    newValue->type = SYMBOL_TYPE;
                    newValue->s = "-";
                    isSymbol = 1;
                    tokensList = cons(newValue, tokensList);
                    newValue = talloc(sizeof(Value));
                    newValue->type = CLOSE_TYPE;
                } else if(!isdigit(nextChar)){
                    
                    printf("Error! Not a valid integer or double.\n");
                    g_error = 1;
                    //exit(0);
                    
                }
            }
            if (isSymbol == 0){
                //double or integer
                long digits;
                double digits_double;
                char str[300];
                char *ptr;
                int j = 0;  
                str[0] = nextChar;
                nextChar = (char)fgetc(stdin);

                while(isdigit(nextChar) || nextChar == '.'){
                    j = j + 1;
                    str[j] = nextChar;   
                    nextChar = (char)fgetc(stdin);
                    //printf("%s\n", str);
                }
                str[j+1]  = '\0';
                if (nextChar != ' '&& nextChar != '\n' && nextChar != ')'){
                    printf("Error! Not a valid integer or double.\n");
                    g_error = 1;
                    //exit(0);
                }
                
                int isDouble = 0;
                for (int i = 0; i < 300; i++){
                    if (str[i] == '.' && isDouble == 0){
                        isDouble = 1;
                    } else if (str[i] == '\0'){
                        break;
                    } else if (str[i] == '.' && isDouble != 0){
                        printf("Error! Multiple decimal points detected.\n");
                        g_error = 1;
                        //exit(0);
                    }
                }
                
                if (isDouble == 1){
                    newValue->type = DOUBLE_TYPE;
                    digits_double = strtod(str, &ptr);
                    newValue->d = digits_double;
                    if (isNegative == 1){
                        newValue->d = digits_double * -1;
                    }
                } else {
                    newValue->type = INT_TYPE;
                    digits = strtol(str, &ptr, 10);
                    newValue->i = digits;
                    if (isNegative == 1){
                        newValue->i = digits * -1;
                    }
                }
                if (nextChar == ')'){
                    tokensList = cons(newValue, tokensList);
                    newValue = talloc(sizeof(Value));
                    newValue->type = CLOSE_TYPE;
                }
            }
            valid = 1;
            
        } else if (nextChar == '"') { //done
            //string
            char *str = talloc(sizeof(char)*300);
            
            int j = 0;
            nextChar = (char)fgetc(stdin);
            while(nextChar != '"') {
                str[j] = nextChar; 
                j = j + 1;
                nextChar = (char)fgetc(stdin);
            }
            str[j] = '\0';
            //"foo::"
            newValue->type = STR_TYPE; 
            newValue->s = str;
            valid = 1;
            
        } else if (nextChar == '+' || nextChar == '=' || nextChar == '/' || nextChar == '*' || nextChar == '<' || nextChar == '>') {
            //symbol case 1
            char *sym = talloc(sizeof(char)*2);
            sym[0] = nextChar;
            sym[1] = '\0';
            newValue->type = SYMBOL_TYPE;
            newValue->s = sym;
            char nextChar = (char)fgetc(stdin);
            if (nextChar == ')') {
                tokensList = cons(newValue, tokensList);
                newValue = talloc(sizeof(Value));
                newValue->type = CLOSE_TYPE;
            } else if (nextChar != ' ') {
                printf("invalid input syntax");
                g_error = 1;
                //exit(0);
            }

            valid  = 1;

        } else if (isalpha(nextChar)) {
            //symbol case 2
            char *sym = talloc(sizeof(char)*300);
            int j = 0;
            while (nextChar == '?' || nextChar == '-'|| nextChar == '*' || nextChar == '!' || isalpha(nextChar) || isdigit(nextChar)) {
                sym[j] = nextChar;
                j = j + 1;
                nextChar = (char)fgetc(stdin);
            }
            sym[j] = '\0';
            newValue->type = SYMBOL_TYPE;
            newValue->s = sym;
            valid = 1;
            if(nextChar == ')'){
                tokensList = cons(newValue, tokensList);
                newValue = talloc(sizeof(Value));
                newValue->type = CLOSE_TYPE;
            }

        } else if (nextChar == ';'){
            //comments
            while (nextChar != '\n'){
                nextChar = (char)fgetc(stdin);
            }
        } else if (nextChar != ' '&& nextChar != '\n'){
            // deal with error syntax that are not space/new line
            printf("Syntax error (readSymbol): symbol ");
            printf("%c", nextChar);
            printf(" does not start with an allowed first character.\n");
            g_error = 1;
            //exit(0);

        }        
        if (valid == 1 && g_error == 0){
            tokensList = cons(newValue, tokensList);
        }
        if (g_error == 1){
            tfree(newValue);
            exit(0);
        }
        // Read next character
        nextChar = (char)fgetc(stdin);
    }
    //displayTokens(tokensList);
    // Reverse the tokens list, to put it back in order
    Value *reversedList = reverse(tokensList);
    return reversedList;
}


// Display the contents of the list of tokens, along with associated type information.
// The tokens are displayed one on each line, in the format specified in the instructions.
// void displayTokens(Value *list){
//     printf("hello");
// }
void displayTokens(Value *list){
    if (g_error != 0){
        return;
    }
    Value *curr = list;
    while (curr->type == CONS_TYPE){
        
        Value *curr_car = curr->c.car;
        if (curr_car != NULL){
            switch (curr_car->type) {
                case INT_TYPE:
                    printf("%d:integer\n", curr_car->i);
                    break;
                case DOUBLE_TYPE:
                    printf("%lf:double\n", curr_car->d);
                    break;
                case STR_TYPE:
                    printf("%c",'"');
                    printf("%s",curr_car->s);
                    printf("%c",'"');
                    printf(":string\n");
                    break;
                case OPEN_TYPE:
                    printf("(:open\n");
                    break;
                case CLOSE_TYPE:
                    printf("):close\n");
                    break;
                case BOOL_TYPE:
                    //int curr_bool = curr->c.car->i;
                    if (curr_car->i == 0){
                        printf("#f:boolean\n");
                    } else {
                        printf("#t:boolean\n");
                    }
                    break;
                case SYMBOL_TYPE:
                    printf("%s:symbol\n", curr_car->s);
                    break;
                case NULL_TYPE:
                    //printf("%s", list->c.car->s);
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
        curr = curr->c.cdr;
    }
}
