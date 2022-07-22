#ifndef _EVAL_DEF
#define _EVAL_DEF

// modifies the current active frame. It creates a new binding 
// of the variable to the corresponding expression
Value *evalDef(Value *args, struct Frame *frame);

#endif
