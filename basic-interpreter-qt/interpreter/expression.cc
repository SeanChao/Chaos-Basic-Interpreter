#include "expression.h"
#include <string>

Expression::Expression() {}
// Expression::Expression(std::string& expr) {}

Expression::~Expression() {}

int Expression::getConstantValue() {}
std::string Expression::getIdentifierName() {}
std::string Expression::getOperator() {}
Expression* Expression::getLHS() {}
Expression* Expression::getRHS() {}

/* ##############
 * ConstantExp
 * ##############
 */
ConstantExp::ConstantExp(int val) : value(val) {}

// int ConstantExp::eval(EvaluationContext& context) { return value; }

std::string ConstantExp::toString() { return std::to_string(value); }

ExpressionType ConstantExp::type() { return CONSTANT; }

int ConstantExp::getConstantValue() { return value; }

/* ##############
 * IdentifierExp
 * ##############
 */
IdentifierExp::IdentifierExp(std::string name) : name(name) {}

// int IdentifierExp::eval(EvaluationContext& context) {
//     // if (!context.isDefined(name)) error(name + " is undefined");
//     return context.getValue(name);
// }

std::string IdentifierExp::toString() { return name; }

ExpressionType IdentifierExp::type() { return IDENTIFIER; }

std::string IdentifierExp::getIdentifierName() { return name; }

/* ##############
 * CompoundExp
 * ##############
 */
CompoundExp::CompoundExp(std::string op, Expression* lhs, Expression* rhs)
    : op(op), lhs(lhs), rhs(rhs) {}

CompoundExp::~CompoundExp(){}
// int CompoundExp::eval(EvaluationContext& context) {
//     int right = rhs->eval(context);
//     if (op == "=") {
//         context.setValue(lhs->getIdentifierName(), right);
//         return right;
//     }
//     int left = lhs->eval(context);
//     if (op == "+") return left + right;
//     if (op == "-") return left - right;
//     if (op == "*") return left * right;
//     if (op == "/") {
//         // if (right == 0) error("Division by 0");
//         return left / right;
//     }
//     // error("Illegal operator in expression");
//     return 0;
// }

std::string CompoundExp::toString() { return op; }

ExpressionType CompoundExp::type() { return COMPOUND; }

std::string CompoundExp::getOperator() { return op; }

Expression* CompoundExp::getLHS() { return lhs; }
Expression* CompoundExp::getRHS() { return rhs; }
