#include <iostream>
#include <map>
#include <string>
#include "common.h"

// const std::string OPERATORS[] = {"+", "-", "*", "/", "=", "(", ")"};
// const int OPERATOR_SIZE = 7;

bool isOp(std::string &c) {
    for (int i = 0; i < OPERATOR_SIZE; i++) {
        if (OPERATORS[i] == c) return true;
    }
    return false;
}

/**
 * compare the precedence of two operators
 *
 * return a positive number if op1>op2, a negative number if op1<op2, 0 if
 * op1=op2
 *
 * @param op1 operator1
 * @param op2 operator2
 * @return precedence
 */
int precedenceCmp(std::string &op1, std::string &op2) {
    std::map<std::string, int> preMap;
    preMap["("] = preMap[")"] = 200;
    preMap["*"] = 100;
    preMap["/"] = 100;
    preMap["+"] = 90;
    preMap["-"] = 90;
    preMap["="] = 20;
    try {
        return preMap[op1] - preMap[op2];
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
