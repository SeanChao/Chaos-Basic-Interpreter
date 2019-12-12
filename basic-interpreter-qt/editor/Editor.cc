#include "Editor.h"
#include <QDebug>
#include <QString>
#include <iostream>
#include <regex>
#include <sstream>
#include "../interpreter/statement.h"

using namespace std;

bool charCompareI(char, char);
bool stringCompareI(const string &str1, const string &str2);

Editor::Editor() { buffer = new ListBuffer(); }

Editor::Editor(Program *p) : Editor() {
    program = p;
    connect(p, &Program::printToGui, this, &Editor::getProgramOutput);
}

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

/*
      cmd-1: non-negative integer + statement (e.g., 100 let a = b + c)
      cmd-2: delete (e.g., d 100)
      cmd-3: list   (e.g., lisT)
      cmd-4: save as a file (e.g., save code.sh)
      cmd-5: run    (running the program)
      cmd-6: clear  (clear the program)
*/
void Editor::dispatchCmd(const string &cmd) {
    stringstream ss(cmd);
    string type;
    int line = 0;

    const string strList = "list";
    if (stringCompareI(strList, cmd)) {
        Editor::cmdList();
        // emit editorOutput("cmd >");
        return;
    }

    // delete function is deleted
    // if (cmd[0] == 'd' && cmd[1] == ' ') {
    //     if (ss.eof()) throw QString("Bad/Unknown command");
    //     ss >> type >> line;
    //     cmdDelete(line);
    //     // emit editorOutput("cmd >");
    //     return;
    // }

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

    if (stringCompareI(type, "quit")) {
        exit(0);
    }

    if (stringCompareI(cmd.substr(0, 6), "clear")) {
        Program *tmp = program;
        program = new Program;
        delete tmp;
        ListBuffer *btmp = buffer;
        buffer = new ListBuffer;
        delete btmp;
        return;
    }

    if (type == "run") {
        qDebug() << "program begin to run ...";
        try{
            program->run();
        }catch(const char *msg) {
            emit editorOutput(msg);
        }catch(QString msg) {
            emit editorOutput(msg);
        }
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

        std::stringstream stmtStream(statement);
        std::string cmd;
        stmtStream >> cmd;
        std::string stmtContent = statement.length() <= cmd.length()
                                      ? ""
                                      : statement.substr(cmd.length() + 1);
        if (stringCompareI(cmd, "LET")) {
            LetStmt *stmt = new LetStmt(stmtContent);
            cmdInput(line, statement);
            program->addStmt(line, stmt);
            std::cout << "[insert] " << line << "\tLET" << stmtContent << "\n";
        } else if (stringCompareI(cmd, "PRINT")) {
            PrintStmt *stmt = new PrintStmt(stmtContent);
            cmdInput(line, statement);
            program->addStmt(line, stmt);
            std::cout << "[insert] " << line << "\tPRINT" << stmtContent
                      << "\n";

        } else if (stringCompareI(cmd, "INPUT")) {
            InputStmt *stmt = new InputStmt(stmtContent);
            cmdInput(line, statement);
            program->addStmt(line, stmt);
        } else if (stringCompareI(cmd, "GOTO")) {
            int jmpTarget;
            stmtStream >> jmpTarget;
            GotoStmt *stmt = new GotoStmt(jmpTarget);
            program->addStmt(line, stmt);
            cmdInput(line, statement);

        } else if (stringCompareI(cmd, "IF")) {
            std::regex ifReg(
                "\\s*(\\w+)\\s*(<|=|>)\\s*(\\w+)\\s+((t|T)(h|H)(e|E)("
                "n|N))\\s+(\\d+)");
            std::smatch match;
            bool flag = std::regex_match(stmtContent, match, ifReg);
            std::cout << "[info] if evaluation " << stmtContent << "\t" << flag
                      << "\n";
            if (!flag)
                throw "IF statement error";
            else {
                const int offset = 1;
                std::string exp1 = match.str(offset);
                std::string exp2 = match.str(offset + 2);
                std::string op = match.str(offset + 1);
                std::string line_str = match.str(offset + 8);
                std::cout << "[info] if evaluation exp1: " << exp1
                          << "\t op:" << op << "\texp2: " << exp2
                          << "\tline: " << line_str << "\n";
                int jmpLine = std::stoi(line_str);
                IfStmt *stmt = new IfStmt(exp1, exp2, op, jmpLine);
                program->addStmt(line, stmt);
                cmdInput(line, statement);
            }
        } else if (stringCompareI(cmd, "END")) {
            EndStmt *stmt = new EndStmt();
            program->addStmt(line, stmt);
            std::cout << "[insert] " << line << "\tEND" << stmtContent << "\n";
            cmdInput(line, statement);

        } else if (stringCompareI(cmd, "REM")) {
            RemStmt *stmt = new RemStmt(stmtContent);
            program->addStmt(line, stmt);
            // std::cout << "add REM:\t" << stmt;
            std::cout << "[insert] " << line << "\tREM" << stmtContent << "\n";
            cmdInput(line, statement);
            return;
        } else {
            throw QString("No such statements");
        }

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

void Editor::getProgramOutput(QString str) {
    qDebug() << "GET IN Program: " << str;
    emit editorOutput(str);
}

bool charCompareI(char a, char b) { return (toupper(a) == toupper(b)); }

bool stringCompareI(const string &str1, const string &str2) {
    return (str1.length() == str2.length() &&
            equal(str1.begin(), str1.end(), str2.begin(), charCompareI));
}
