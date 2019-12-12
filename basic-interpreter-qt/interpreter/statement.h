#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include "expression.h"

enum StmtType { REM, LET, PRINT, INPUT, GOTO, IF, END };

class Statement {
   private:
   public:
    Statement();
    virtual ~Statement();
    virtual StmtType type() const;
    virtual Expression *getExp() const;
};

class RemStmt : public Statement {
   private:
    std::string msg;

   public:
    RemStmt(std::string &rem);
    virtual ~RemStmt();
    virtual StmtType type() const;
};

// TODO: check the necessity of identifier
class LetStmt : public Statement {
   private:
    IdentifierExp *identifier;
    Expression *exp;

   public:
    LetStmt(std::string &stmt);
    virtual ~LetStmt();
    virtual StmtType type() const;
    IdentifierExp *getIdentifier() const;
    Expression *getExp() const;
};

class PrintStmt : public Statement {
   private:
    // IdentifierExp *identifier;
    Expression *expression;

   public:
    PrintStmt(std::string &expression);
    virtual ~PrintStmt();
    virtual StmtType type() const;
    virtual Expression *getExp() const;
};

class InputStmt : public Statement {
   private:
    IdentifierExp *identifier;
    int val;

   public:
    InputStmt(std::string &identifier);
    virtual ~InputStmt();
    virtual StmtType type() const;
    virtual Expression *getExp() const;
    int getVal() const;
};

class GotoStmt : public Statement {
   private:
    int line;
    ConstantExp *lineExp;

   public:
    GotoStmt(int line);
    virtual ~GotoStmt();
    virtual StmtType type() const;
    virtual Expression *getExp() const;
};

class IfStmt : public Statement {
   private:
    Expression *exp1;
    Expression *exp2;
    std::string op;
    // CompoundExp *exp;
    int line;

   public:
    IfStmt(std::string &exp1, std::string &exp2, std::string op, int line);
    virtual ~IfStmt();
    virtual StmtType type() const;
    Expression *getExp(int i) const;
    std::string getOp() const;
    int getJmpLine() const;
};

class EndStmt : public Statement {
   private:
   public:
    EndStmt();
    virtual ~EndStmt();
    virtual StmtType type() const;
};

#endif
