#pragma once

#include <string>

using namespace std;

class ListBuffer {
private:
    struct node {
        int line;
        string statement;
        node *next;
        node(): next(NULL) {}
        node(int line, const string &text) {
            this -> line = line;
            this -> statement = text;
        }
        node(int line, const string &text, node *next): line(line), statement(text), next(next) { }
            
    };
        node *head;

public:
    ListBuffer();
    ~ListBuffer();

    void writeToFile(const string &filename) const;
    void showLines() const;
    void deleteLine(int line_idx);
    void insertLine(int line_idx, const string &text);
};
