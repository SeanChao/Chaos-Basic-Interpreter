#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <map>
#include <string>

const std::string OPERATORS[] = {"+", "-", "*", "/", "=", "(", ")"};
const int OPERATOR_SIZE = 7;

bool isOp(std::string &c);

int precedenceCmp(std::string &op1, std::string &op2);

#endif