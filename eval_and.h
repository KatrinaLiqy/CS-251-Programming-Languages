#ifndef _EVAL_AND
#define _EVAL_AND


//Evaluates both arguments and returns #t if both are true, and #f in all other cases
Value *evalAnd(Value *args, Frame *frame);

#endif