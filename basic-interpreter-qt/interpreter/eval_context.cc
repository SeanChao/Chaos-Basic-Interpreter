#include "eval_context.h"
#include <iostream>

EvalContext::EvalContext() {}

EvalContext::~EvalContext() {}

void EvalContext::setVar(std::string &var, int value) {
    symbolMap.insert(std::pair<std::string, int>(var, value));
}

/**
 * return the value of given var which exists.
 */
int EvalContext::getVar(std::string &var) const {
    std::map<std::string, int>::const_iterator iter;
    iter = symbolMap.find(var);
    return iter->second;
}

bool EvalContext::isDefined(std::string &var) const {
    std::map<std::string, int>::const_iterator iter;
    iter = symbolMap.find(var);
    if (iter == symbolMap.end()) {
        std::cout << "identifier not found." << std::endl;
        return false;
    } else {
        return true;
    }
}
