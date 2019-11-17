#include "Editor.h"
#include <iostream>
#include <sstream>

using namespace std;

bool charCompareI(char, char);
bool stringCompareI(const string &str1, const string &str2);

Editor::Editor() { buffer = new ListBuffer(); }

Editor::~Editor() { delete buffer; }

void Editor::run() {
    // TODO: turn cout in outputstream...
    string cmd;
    while (true) {
        qDebug() << "cmd> ";
        // qDebug().flush();
        getline(cin, cmd);
        if (cmd == "Q") break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            qDebug() << "? " << e << endl;
        } catch (const out_of_range &oor) {
            qDebug() << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            qDebug() << "? " << re.what() << endl;
        }
    }
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
        return;
    }

    if (cmd[0] == 'd' && cmd[1] == ' ') {
        if (ss.eof()) throw "Bad/Unknown command\n";
        ss >> type >> line;
        cmdDelete(line);
        return;
    }

    type = cmd.substr(0, 5);
    if (type == "save ") {
        if (cmd.length() == 5) throw "No filename specified\n";
        string filename = "";
        for (int i = 5; i < cmd.length(); i++) {
            filename.append(1, cmd[i]);
        }
        cmdSave(filename);
        return;
    }

    ss >> line;
    if (!ss.eof() && line > 0) {
        string statement = "";
        int lineStrLength = numberLength(line);
        if (cmd[lineStrLength] != ' ') throw "Bad/Unknown command\n";
        for (int i = lineStrLength + 1; i < cmd.length(); i++) {
            statement += cmd[i];
        }
        cmdInput(line, statement);
        return;
    }

    throw "Bad/Unknown command\n";
}

void Editor::cmdDelete(const int line) { buffer->deleteLine(line); }

void Editor::cmdInput(int line, const string &statement) {
    buffer->insertLine(line, statement);
}

void Editor::cmdList() const { buffer->showLines(); }

void Editor::cmdSave(const string &filename) const {
    buffer->writeToFile(filename);
}

// TODO: add other code you want
int Editor::numberLength(int number) {
    if (number == 0) return 0;
    return 1 + numberLength(number / 10);
}

bool charCompareI(char a, char b) { return (toupper(a) == toupper(b)); }

bool stringCompareI(const string &str1, const string &str2) {
    return (str1.length() == str2.length() &&
            equal(str1.begin(), str1.end(), str2.begin(), charCompareI));
}