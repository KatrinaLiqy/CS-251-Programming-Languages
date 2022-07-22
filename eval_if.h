#ifndef _EVAL_IF
#define _EVAL_IF

//(if test trueExpr falseExpr), where the evaluation 
//of trueExpr is returned if test holds (evaluates to 
//a truthy value), else the evaluation of falseExpr is returned
//we parse the args so that arg1 is test, arg2 is trueExpr, arg3 is falseExpr
Value *evalIf(Value *args, Frame *frame);

#endif
