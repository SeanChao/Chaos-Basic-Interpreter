#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "common.cc"
#include "eval_context.cc"
#include "expression.cc"
#include "parser.cc"
#include "program.cc"
#include "statement.cc"
#include "tokenizer.cc"

bool charCompareI(char a, char b) { return (toupper(a) == toupper(b)); }

bool stringCompareI(const std::string &str1, const std::string &str2) {
    return (str1.length() == str2.length() &&
            equal(str1.begin(), str1.end(), str2.begin(), charCompareI));
}

int main(int argc, char const *argv[]) {
    std::string testArr[] = {
        "a + 1",  "a + b", "a = b + 1", "c=a-b +1", "Odd = 1 + n * (2 -x) / 2 ",
        "print a"};
    int testSize = 5;

    // tokenizer test
    for (int i = 0; i < testSize; i++) {
        Tokenizer t(testArr[i]);
        std::vector<std::string> tokenList = t.getTokenList();
        int idx = 0;
        for (std::string token : tokenList) {
            std::cout << idx++ << '\t' << token << "\n";
        }
    }

    // parser test
    Parser parser;
    // for (int i = 0; i < testSize; i++) {
    //     parser.parse(testArr[i]);
    // }
    try {
        parser.parse(testArr[4]);
    } catch (const char *e) {
        std::cout << e << "\n";
    }

    // read from file
    std::ifstream file;
    std::string codeline;
    file.open("example.b");
    int line = 0;
    Program *program = new Program;

    while (getline(file, codeline)) {
        line += 10;
        std::cout << "[in] " << line << "\t" << codeline << "\n";
        std::string statement = codeline;
        // using namespace std;

        std::stringstream stmtStream(statement);
        std::string cmd;
        stmtStream >> cmd;
        std::string stmtContent = statement.length() <= cmd.length()
                                      ? ""
                                      : statement.substr(cmd.length() + 1);
        if (stringCompareI(cmd, "LET")) {
            LetStmt *stmt = new LetStmt(stmtContent);
            program->addStmt(line, stmt);
            std::cout << "[insert] " << line << "\tLET" << stmtContent << "\n";
        } else if (stringCompareI(cmd, "PRINT")) {
            PrintStmt *stmt = new PrintStmt(stmtContent);
            program->addStmt(line, stmt);
            std::cout << "[insert] " << line << "\tPRINT" << stmtContent
                      << "\n";

        } else if (stringCompareI(cmd, "INPUT")) {
            InputStmt *stmt = new InputStmt(stmtContent);
            program->addStmt(line, stmt);
        } else if (stringCompareI(cmd, "GOTO")) {
            int jmpTarget;
            stmtStream >> jmpTarget;
            GotoStmt *stmt = new GotoStmt(jmpTarget);
            program->addStmt(line, stmt);
        } else if (stringCompareI(cmd, "IF")) {
            std::regex ifReg(
                "\\s*(\\w+)\\s*(<|=|>)\\s*(\\w+)\\s+((t|T)(h|H)(e|E)("
                "n|N))\\s+(\\d+)");
            std::smatch match;
            bool flag = std::regex_match(stmtContent, match, ifReg);
            std::cout << "[info] if evaluation " << stmtContent << "\t" << flag << "\n";
            if (!flag)
                throw "IF statement error";
            else {
                const int offset = 1;
                std::string exp1 = match.str(offset);
                std::string exp2 = match.str(offset+2);
                std::string op = match.str(offset+1);
                std::string line_str = match.str(offset + 8);
                std::cout << "[info] if evaluation exp1: " << exp1 << "\t op:" << op
                          << "\texp2: " << exp2 << "\tline: " << line_str << "\n";
                int jmpLine = std::stoi(line_str);
                IfStmt *stmt = new IfStmt(exp1, exp2, op, jmpLine);
                program->addStmt(line, stmt);
            }
        } else if (stringCompareI(cmd, "END")) {
            EndStmt *stmt = new EndStmt();
            program->addStmt(line, stmt);
            std::cout << "[insert] " << line << "\tEND" << stmtContent << "\n";

        } else if (stringCompareI(cmd, "REM")) {
            RemStmt *stmt = new RemStmt(stmtContent);
            program->addStmt(line, stmt);
            // std::cout << "add REM:\t" << stmt;
            std::cout << "[insert] " << line << "\tREM" << stmtContent << "\n";
        }
    }
    file.close();
    std::cout << "file input successfully\n";
    program->run();
    return 0;
}
