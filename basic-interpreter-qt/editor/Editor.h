#pragma once

#include <QObject>
#include <string>
#include "../interpreter/program.h"
#include "ListBuffer.h"

class Editor : public QObject {
    Q_OBJECT
   public:
    Editor();
    Editor(Program *);
    ~Editor();
    void run(QString cmd);

   signals:
    void editorOutput(QString str) const;
    void setInput(int val);

   public slots:
    void getUserInput(QString str);
    void getProgramOutput(QString str);
    void setInputFlag(bool state);

   private:
    Program *program;
    ListBuffer *buffer;

    void dispatchCmd(const string &cmd);
    void cmdDelete(int line);
    void cmdList() const;
    void cmdInput(int line, const string &statement);
    void cmdSave(const string &filename) const;

    int numberLength(int number);
    bool inputState;
};
