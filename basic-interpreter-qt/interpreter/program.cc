#include "program.h"
#include <iostream>
#include <list>

Program::Program() {}

Program::~Program() {
    for(auto i: stmtTree) {
        delete i.stmt;
    }
}

void Program::addSourceLine(int line, std::string stmt) {}

void Program::addStmt(int line, Statement *stmt) {
    // insert statement
    Node node(line, stmt);
    if (line > stmtTree.end()->line) {
        stmtTree.push_back(node);
        return;
    }
    auto it = stmtTree.begin();
    while (!stmtTree.empty() && (*it).line < line) {
        it++;
    }
    std::cout << "insert at " << (*it).line << "\n";
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
        // std::cout << "execution " << iCount << "\n";
        if (iCount >= 20) {
            std::cout << "loop endless ...\n";
            return;
        }
        Statement *statement = (*it).stmt;
        std::cout << "[info] running line " << (*it).line << "\n";
        std::cout << "[debug] " << statement->type() << "\n";
        switch ((*it).stmt->type()) {
            case REM:
                it++;
                break;
            case LET: {
                // ((LetStmt *)statement)->getExp()->eval(evalContext);
                // std::cout << "LET... \n";
                int val = statement->getExp()->eval(evalContext);
                std::cout << "[out] (LET)" << val << "\n";
                it++;
                break;
            }
            case PRINT: {
                int val = statement->getExp()->eval(evalContext);
                std::cout << "[out] " << val << "\n";
                QString ret = QString::number(val);
                emit printToGui(ret);
                it++;
                break;
            }
            case INPUT: {
                std::string identifierName = statement->getExp()->toString();
                // TODO: get input from user
                std::cout << "?: ";
                int val;
                std::cin >> val;
                evalContext.setVar(identifierName, val);
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
                }
                else it++;
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
            break;
        }
    }
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
