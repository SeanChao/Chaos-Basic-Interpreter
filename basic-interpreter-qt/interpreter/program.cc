#include "program.h"
#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <list>

Program::Program() : inputState(false) {}

Program::~Program() {
    for (auto i : stmtTree) {
        delete i.stmt;
    }
    evalContext.reset();
}

void Program::addSourceLine(int line, std::string stmt) {}

void Program::addStmt(int lineNumber, Statement *stmt) {
    // debug:
    // qDebug() << "add to line " << lineNumber;
    for (auto it : stmtTree) {
        qDebug() << "line " << it.line << "stmt " << it.stmt;
    }

    // insert statement
    Node node(lineNumber, stmt);

    if (stmtTree.empty()) {
        stmtTree.push_back(node);
        return;
    }

    auto it = stmtTree.begin();
    while (!stmtTree.empty() && (*it).line < lineNumber) {
        it++;
        qDebug() << "[debug] visit line" << (*it).line << " to find "
                 << lineNumber;
        if (it == stmtTree.end()) {
            stmtTree.push_back(node);
            qDebug() << "insert to end";
            return;
        }
    }
    qDebug() << "[debug] visit line" << (*it).line << " -> " << lineNumber;
    if ((*it).line == lineNumber) {
        // modify a line in program
        Statement *tmp = (*it).stmt;
        (*it).stmt = stmt;
        delete tmp;
        qDebug() << "modify line " << lineNumber << " to be " << stmt;
        return;
    }
    std::cout << "insert at " << (*it).line << "\n";
    qDebug() << "insert at " << (*it).line << "\n";
    stmtTree.insert(it, node);
}

void Program::run() {
    if (stmtTree.empty()) return;
    std::list<Node>::iterator it = stmtTree.begin();
    std::cout << "program begin to run\n";
    int iCount = 0;
    while ((*it).stmt != NULL && (*it).stmt->type() != END) {
        // execution
        ++iCount;
        Statement *statement = (*it).stmt;
        std::cout << "[info] running line " << (*it).line << "\n";
        // std::cout << "[debug] " << statement->type() << "\n";
        switch ((*it).stmt->type()) {
            case REM:
                it++;
                break;
            case LET: {
                int val = statement->getExp()->eval(evalContext);
                std::cout << "[out] (LET)" << val << "\n";
                qDebug() << "[out] (LET)" << val << "\n";
                it++;
                break;
            }
            case PRINT: {
                int val = statement->getExp()->eval(evalContext);
                std::cout << "[out] printed: " << val << "\n";
                qDebug() << "[out] printed: " << val << "\n";
                QString ret = QString::number(val);
                emit printToGui(ret);
                it++;
                break;
            }
            case INPUT: {
                inputState = true;
                emit changeEditorInputState(inputState);
                std::string identifierName = statement->getExp()->toString();
                emit printToGui("? ");
                /** CLI mode: following code without Qt would suffice
                 * std::cout << "?: ";
                 * int val = getInput();
                 * std::cin >> val;
                 */
                while (inputState) {
                    QCoreApplication::processEvents();
                    // qDebug() << "Blocked in input state in program";
                }
                qDebug() << "end of user input";
                evalContext.setVar(identifierName, inputVal);
                it++;
                break;
            }
            case GOTO: {
                int jmpLine = statement->getExp()->eval(evalContext);
                std::cout << "[tip] "
                          << "try jump to" << jmpLine << "\n";
                // reset iterator
                std::list<Node>::iterator tmp = stmtTree.begin();
                while ((*tmp).line < jmpLine) {
                    tmp++;
                    std::cout << "[tip] "
                              << "loop to " << (*tmp).line << "\n";
                }
                std::cout << "[tip] "
                          << "loop end at " << (*tmp).line << "\n";
                if ((*tmp).line == jmpLine) {
                    it = tmp;
                    std::cout << "[tip] "
                              << "jump->" << (*it).line << " " << (*it).stmt
                              << "\n";
                } else {
                    std::cout << "Goto target doesn't exist. Skipped.\n";
                    it++;
                }

                break;
            }
            case IF: {
                IfStmt *stmt = (IfStmt *)statement;
                int val0 = stmt->getExp(0)->eval(evalContext);
                std::cout << "[info] if test:6001 " << val0 << "\n";
                int val1 = stmt->getExp(1)->eval(evalContext);
                std::cout << "[info] if test:6002 \n";
                std::string op = stmt->getOp();
                int line = stmt->getJmpLine();
                bool flag = false;
                if (op == "=") {
                    flag = val0 == val1;
                } else if (op == "<") {
                    flag = val0 < val1;
                } else if (op == ">") {
                    flag = val0 > val1;
                }
                std::cout << "[info] if test: " << val0 << " " << val1 << " "
                          << op << flag << "\n";
                if (flag) {
                    // jump(it, line)
                    int jmpLine = line;
                    std::list<Node>::iterator tmp = stmtTree.begin();
                    while ((*tmp).line < jmpLine) {
                        tmp++;
                        std::cout << "[tip] "
                                  << "loop to " << (*tmp).line << "\n";
                    }
                    std::cout << "[tip] "
                              << "loop end at " << (*tmp).line << "\n";
                    if ((*tmp).line == jmpLine) {
                        it = tmp;
                        std::cout << "[tip] "
                                  << "jump->" << (*it).line << " " << (*it).stmt
                                  << "\n";
                    } else {
                        std::cout << "Goto target doesn't exist. Skipped.\n";
                        it++;
                    }
                } else
                    it++;
                break;
            }
            case END:
                std::cout << "Program finished with exit code 0."
                          << "\n";
                break;
            default:
                it++;
                break;
        }

        // if no "END" available
        if (it == stmtTree.end()) {
            std::cout << "[warning] No END\n";
            evalContext.reset();
        qDebug() << "Context reset";
            throw "[warning] No END";
        }
    }
    evalContext.reset();
    qDebug() << "Context reset";
}

bool Program::jump(std::list<Node>::iterator &it, int jmpLine) {
    std::cout << "[tip] "
              << "try jump to" << jmpLine << "\n";
    // reset iterator
    std::list<Node>::iterator tmp = stmtTree.begin();
    while ((*tmp).line < jmpLine) {
        tmp++;
        std::cout << "[tip] "
                  << "loop to " << (*tmp).line << "\n";
    }
    std::cout << "[tip] "
              << "loop end at " << (*tmp).line << "\n";
    if ((*tmp).line == jmpLine) {
        it = tmp;
        std::cout << "[tip] "
                  << "jump->" << (*it).line << " " << (*it).stmt << "\n";
        return true;
    } else {
        std::cout << "Goto target doesn't exist. Skipped.\n";
        it++;
        return false;
    }
}

void Program::getInput(int val) {
    inputState = false;
    inputVal = val;
    // reset input state
    // emit printToGui("[debug] in getInput");
    emit changeEditorInputState(inputState);
}

// void Program::setInputState(bool state) { inputState = state; }
