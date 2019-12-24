#ifndef PROGRAM_H
#define PROGRAM_H
#include <QObject>
#include <QString>
#include <list>
#include "eval_context.h"
#include "statement.h"

class Program : public QObject {
    Q_OBJECT
   private:
    struct Node {
        Node(int line, Statement *stmt) : line(line), stmt(stmt) {}
        int line;
        Statement *stmt;
    };

    std::list<Node> stmtTree;
    EvalContext evalContext;
    bool jump(std::list<Node>::iterator &it, int line);
    int inputVal;

   public:
    Program();
    ~Program();

    void addSourceLine(int line, std::string stmt);
    void addStmt(int line, Statement *stmt);
    void run();
    bool inputState;

   signals:
    void printToGui(QString str);
    void changeEditorInputState(bool state);
   
   public slots:
    void getInput(int val);
    // void setInputState(bool state);
};

#endif
