#ifndef EVAL_CONTEXT_H
#define EVAL_CONTEXT_H

#include <map>

/*
 * Class: EvalContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 */
class EvalContext {
   private:
    std::map<std::string, int> symbolMap;

   public:
    EvalContext();
    ~EvalContext();
    void setVar(std::string &var, int value);
    int getVar(std::string &var) const;
    bool isDefined(std::string &var) const;
    void reset();
};

#endif