#include "Editor.h"
#include <QDebug>
#include <QString>
#include <iostream>
#include <sstream>

using namespace std;

bool charCompareI(char, char);
bool stringCompareI(const string &str1, const string &str2);

Editor::Editor() { buffer = new ListBuffer(); }

Editor::~Editor() { delete buffer; }

void Editor::run(QString cmd) {
    // emit editorOutput("cmd> ");
    // qDebug().flush();
    if (cmd == "Q") return;
    try {
        dispatchCmd(cmd.toUtf8().constData());
    } catch (const QString &e) {
        // qDebug() << "? " << e << endl;
        // QString info = ;
        editorOutput(e);
    } catch (const out_of_range &oor) {
        qDebug() << "? " << oor.what() << endl;
        editorOutput("? ");
    } catch (const range_error &re) {
        qDebug() << "? " << re.what() << endl;
        editorOutput("? ");
    }
    emit editorOutput(QString("cmd >").replace("[[\r]\n]", ""));
}

void Editor::dispatchCmd(const string &cmd) {
    /*
          cmd-1: non-negative integer + statement (e.g., 100 let a = b + c)
          cmd-2: delete (e.g., d 100)
          cmd-3: list   (e.g., lisT)
          cmd-4: save as a file (e.g., save code.sh)
    */
    stringstream ss(cmd);
    string type;
    int line = 0;

    const string strList = "list";
    if (stringCompareI(strList, cmd)) {
        Editor::cmdList();
        // emit editorOutput("cmd >");
        return;
    }

    if (cmd[0] == 'd' && cmd[1] == ' ') {
        if (ss.eof()) throw QString("Bad/Unknown command");
        ss >> type >> line;
        cmdDelete(line);
        // emit editorOutput("cmd >");
        return;
    }

    type = cmd.substr(0, 5);
    if (type == "save ") {
        if (cmd.length() == 5) throw QString("No filename specified");
        string filename = "";
        for (int i = 5; i < cmd.length(); i++) {
            filename.append(1, cmd[i]);
        }
        cmdSave(filename);
        // emit editorOutput("cmd >");
        return;
    }

    ss >> line;
    if (!ss.eof() && line > 0) {
        string statement = "";
        int lineStrLength = numberLength(line);
        if (cmd[lineStrLength] != ' ') throw QString("Bad/Unknown command");
        for (int i = lineStrLength + 1; i < cmd.length(); i++) {
            statement += cmd[i];
        }
        cmdInput(line, statement);
        // emit editorOutput("cmd >");
        return;
    }

    throw QString("Bad/Unknown command");
}

void Editor::cmdDelete(const int line) { buffer->deleteLine(line); }

void Editor::cmdInput(int line, const string &statement) {
    buffer->insertLine(line, statement);
}

void Editor::cmdList() const {
    buffer->showLines();
    int linesCount = buffer->getLength();
    for (int i = 0; i < linesCount; i++) {
        QString lineNumber = QString::number(buffer->getlineNumber(i));
        QString bufferedLine =
            lineNumber + QString::fromStdString("\t" + buffer->getline(i));
        emit editorOutput(bufferedLine);
    }
}

void Editor::cmdSave(const string &filename) const {
    buffer->writeToFile(filename);
}

int Editor::numberLength(int number) {
    if (number == 0) return 0;
    return 1 + numberLength(number / 10);
}

void Editor::getUserInput(QString str) {
    qDebug() << "GET IN ED: " << str;
    run(str);
}

bool charCompareI(char a, char b) { return (toupper(a) == toupper(b)); }

bool stringCompareI(const string &str1, const string &str2) {
    return (str1.length() == str2.length() &&
            equal(str1.begin(), str1.end(), str2.begin(), charCompareI));
}
