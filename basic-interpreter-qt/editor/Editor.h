#pragma once

#include <QObject>
#include <string>
#include "ListBuffer.h"

class Editor : public QObject {
    Q_OBJECT
   public:
    Editor();
    ~Editor();
    void run();

   signals:
    void editorOutput(QString str);

   public slots:
    void getUserInput(QString str);

   private:
    ListBuffer *buffer;

    void dispatchCmd(const string &cmd);
    void cmdDelete(int line);
    void cmdList() const;
    void cmdInput(int line, const string &statement);
    void cmdSave(const string &filename) const;

    int numberLength(int number);
};
