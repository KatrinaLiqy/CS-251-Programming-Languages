#ifndef _BUILT_IN
#define _BUILT_IN


 /* 
 * built_in.c contains the primitive functions as well
 * as the functions that bind the function name to the function itself
 */

Value *builtInNull(Value *args);

Value *builtInCar(Value *args);

Value *builtInCdr(Value *args);

Value *builtInCons(Value *args);

Value *builtInPlus(Value *args);

Value *builtInMinus(Value *args);

Value *builtInMultiply(Value *args);

Value *builtInDivide(Value *args);

Value *builtInModulo(Value *args);

Value *builtInLessThan(Value *args);

// binds the primitive type to the corresponding c function
void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame);

/// calls all the functions that actually bind the primitive type to
// the corresponding c function
void bindAllPrimitiveFunction(Frame *globalFrame);

#endif
