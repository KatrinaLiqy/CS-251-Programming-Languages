#ifndef _EVAL_LET
#define _EVAL_LET

//evaluate a let expression
Value *evalLet(Value *args, struct Frame *frame);

// Evaluates expressions from left to right and then
// the body. The expressions on the right are within
// the scope of the expressions on the left
Value *evalLetAsterisk(Value *args, struct Frame *frame);

// Evaluates expressions in unspecified order and
// returns the result of the body.
Value *evalLetrec(Value *args, struct Frame *frame);

#endif
