#ifndef PARSER_H
#define PARSER_H

#include "expression.h"

class Parser {
   private:
    ExpressionType typeInfer(std::string &token);

   public:
    Parser();
    ~Parser();

    Expression* parse(std::string &expr);
};

#endif