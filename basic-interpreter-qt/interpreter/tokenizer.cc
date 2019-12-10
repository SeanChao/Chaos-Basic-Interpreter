#include "tokenizer.h"
#include "common.h"

#include <iostream>
#include <string>

Tokenizer::Tokenizer(std::string &str) { tokenize(str); }

Tokenizer::~Tokenizer() {}

// TODO: handle the exception of illegal token
void Tokenizer::tokenize(std::string &expr) {
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') {
            expr.erase(i, 1);
        }
    }

    int begin = 0;
    for (int idx = 0; idx < expr.length(); idx++) {
        // std::cout << "scanning " << expr[idx] << "\n";
        // if (expr[idx] == '+' || expr[idx] == '-' || expr[idx] == '*' ||
        //     expr[idx] == '/' || expr[idx] == '=')
        std::string s(1, expr[idx]);
        if (isOp(s)) {
            // if(s=="("){
            //     tokenList.push_back(s);
            //     begin++;
            //     continue;
            // }
            if (idx - begin != 0)  // have other tokens
                tokenList.push_back(expr.substr(begin, idx - begin));
            tokenList.push_back(expr.substr(idx, 1));
            // std::string token1 = expr.substr(begin, idx - begin);
            // std::string token2 = expr.substr(idx, 1);
            // std::cout << "tokenize: " << token1 << "\n";
            // std::cout << "tokenize: " << token2 << "\n";
            // if (token1 != "") tokenList.push_back(token1);
            // if (token2 != "") tokenList.push_back(token2);
            begin = idx + 1;
        } else if (idx == expr.length() - 1) {
            std::string token = expr.substr(begin, idx - begin + 1);
            tokenList.push_back(token);
            // std::cout << "tokenize: " << token << "\n";
        }
    }
}

std::vector<std::string> Tokenizer::getTokenList() const {
    return this->tokenList;
}
