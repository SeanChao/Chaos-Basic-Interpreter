#include "statement.h"
#include <string>
#include "parser.h"

Statement::Statement() {}

Statement::~Statement() {}

Expression *Statement::getExp() const {}

StmtType Statement::type() const {}

RemStmt::RemStmt(std::string &rem) : msg(rem) {}
RemStmt::~RemStmt() {}

StmtType RemStmt::type() const { return REM; }

LetStmt::LetStmt(std::string &stmt) {
    Parser parser;
    Expression *expression = parser.parse(stmt);
    if (expression->type() == IDENTIFIER) {
        throw "No assignment";
        return;
    }
    if (expression->getOperator() != "=") {
        throw "Not an assignment";
        // return;
    }
    exp = expression;
}

LetStmt::~LetStmt() {}

IdentifierExp *LetStmt::getIdentifier() const { return identifier; }

Expression *LetStmt::getExp() const { return exp; }

StmtType LetStmt::type() const { return LET; }

PrintStmt::PrintStmt(std::string &expression) {
    // this->identifier = new IdentifierExp(identifier);
    Parser parser;
    Expression *expr = parser.parse(expression);
    this->expression = expr;
}

PrintStmt::~PrintStmt() {}

StmtType PrintStmt::type() const { return PRINT; }

Expression *PrintStmt::getExp() const { return expression; }

InputStmt::InputStmt(std::string &identifier) {
    this->identifier = new IdentifierExp(identifier);
}

InputStmt::~InputStmt() {}

StmtType InputStmt::type() const { return INPUT; }

Expression *InputStmt::getExp() const { return identifier; }

int InputStmt::getVal() const { return val; }

GotoStmt::GotoStmt(int line) : line(line) { lineExp = new ConstantExp(line); }

GotoStmt::~GotoStmt() {}

StmtType GotoStmt::type() const { return GOTO; }

Expression *GotoStmt::getExp() const { return lineExp; }

IfStmt::IfStmt(std::string &exp1, std::string &exp2, std::string op, int line) {
    Parser p1;
    this->exp1 = p1.parse(exp1);
    // std::cout << "[debug] parsed exp1:" << this->exp1->toString() << "\n";
    Parser p2;
    this->exp2 = p2.parse(exp2);
    // std::cout << "[debug] parsed exp2:" << this->exp2->toString() << "\n";

    this->op = op;
    this->line = line;
    // std::cout << "build if stmt\n";
}

IfStmt::~IfStmt() {}

StmtType IfStmt::type() const { return IF; }

Expression *IfStmt::getExp(int i) const {
    if (i == 0) {
        // std::cout << "[debug] fetched exp1\n";
        return exp1;
    } else if (i == 1) {
        // std::cout << "[debug] fetched exp2\n";
        return exp2;
    }
    return NULL;
}

std::string IfStmt::getOp() const { return op; }

int IfStmt::getJmpLine() const { return line; }

EndStmt::EndStmt() {}

EndStmt::~EndStmt() {}

StmtType EndStmt::type() const { return END; }
