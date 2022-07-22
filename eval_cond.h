#ifndef _EVAL_COND
#define _EVAL_COND

// Checks for the first #t value it encounters and returns the corresponding value.
// If no value is true, it has else as a default case.
Value *evalCond(Value *args, Frame *frame);

#endif
