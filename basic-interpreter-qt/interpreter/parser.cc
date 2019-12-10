#include "parser.h"
#include <stack>
#include "common.h"
#include "expression.h"
#include "tokenizer.h"

Parser::Parser() {}

Parser::~Parser() {}

/**
 * type inference for tokens
 *
 * Types are CONSTANT, IDENTIFIER, COMPOUND.
 *
 * @param std::string &token token to be inferred
 * @return the type of given token
 */
ExpressionType Parser::typeInfer(std::string &token) {
    // CONSTANT
    // TODO: examine every char
    if (token[0] >= '0' && token[0] <= '9') {
        return CONSTANT;
    }
    return IDENTIFIER;
}

Expression *Parser::parse(std::string &expr) {
    // tokenize
    Tokenizer t(expr);
    std::vector<std::string> tokenList = t.getTokenList();

    // parse
    std::stack<std::string> operators;
    std::stack<Expression *> operands;
    for (auto token : tokenList) {
        // parsing operators
        if (isOp(token)) {
            // pop if next op's precedence is above stack top
            if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    std::string op = operators.top();
                    operators.pop();
                    if (operands.size() < 2)
                        throw "Error: invalid Expression, missing operands";
                    Expression *exp1 = operands.top();
                    operands.pop();
                    Expression *exp2 = operands.top();
                    operands.pop();
                    CompoundExp *cExp = new CompoundExp(op, exp2, exp1);
                    operands.push(cExp);
                }
                operators.pop();
                continue;
            }
            while (!operators.empty() &&
                   precedenceCmp(token, operators.top()) <= 0 &&
                   operators.top() != "(") {
                std::string op = operators.top();
                operators.pop();
                if (operands.size() < 2)
                    throw "Error: invalid Expression, missing operands";
                Expression *exp1 = operands.top();
                operands.pop();
                Expression *exp2 = operands.top();
                operands.pop();
                CompoundExp *cExp = new CompoundExp(op, exp2, exp1);
                operands.push(cExp);
            }
            operators.push(token);
        } else {
            // parsing operands
            ExpressionType tokenType = typeInfer(token);
            if (tokenType == CONSTANT) {
                ConstantExp *exp = new ConstantExp(stoi(token));
                operands.push(exp);
            } else if (tokenType == IDENTIFIER) {
                IdentifierExp *exp = new IdentifierExp(token);
                operands.push(exp);
            }
        }
    }
    // every token is scanned
    Expression *root;
    while (!operators.empty()) {
        std::string op = operators.top();
        operators.pop();
        if (operands.size() < 2)
            throw "Error: invalid Expression, missing operands";
        Expression *exp1 = operands.top();
        operands.pop();
        Expression *exp2 = operands.top();
        operands.pop();
        CompoundExp *exp = new CompoundExp(op, exp2, exp1);
        operands.push(exp);
    }
    root = operands.top();
    operands.pop();
    if (!operands.empty()) throw "Error: invalid Expression";

    // debug output
    std::stack<Expression *> exp;
    exp.push(root);
    while (!exp.empty()) {
        Expression *tmp = exp.top();
        // std::cout << "exp " << tmp->toString();
        if (tmp->type() == COMPOUND)
            std::cout << "exp " << tmp->toString() << "\tL "
                      << ((tmp->getLHS()) ? (tmp->getLHS()->toString())
                                          : "NULL")
                      << "\tR "
                      << ((tmp->getRHS()) ? (tmp->getRHS()->toString())
                                          : "NULL")
                      << "\n";
        else
            std::cout << "leaf: " << tmp->toString() << "\n";
        exp.pop();
        if (tmp->type() == COMPOUND && tmp->getRHS()) exp.push(tmp->getRHS());
        if (tmp->type() == COMPOUND && tmp->getLHS()) exp.push(tmp->getLHS());
    }

    return root;
}