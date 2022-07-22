#ifndef _EVAL_OR
#define _EVAL_OR

//Evaluates both arguments and returns #t if even if one is true, and #f when both are #f
Value *evalOr(Value *args, Frame *frame);

#endif