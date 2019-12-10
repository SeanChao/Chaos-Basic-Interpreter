#include <iostream>
#include "expression.cc"
#include "parser.cc"
#include "tokenizer.cc"

// #include "tokenizer.h"

int main(int argc, char const* argv[]) {
    std::string testArr[] = {"a + 1", "a + b", "a = b + 1", "c=a-b +1",
                             "Odd = 1 + n * (2 -x) / 2 "};
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
    } catch (const char* e) {
        std::cout << e << "\n";
    }
    return 0;
}
