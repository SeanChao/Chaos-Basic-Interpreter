#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
// #include "expression.h"

/**
 * Convert a std::string into a list of tokens
 */
class Tokenizer {
   private:
    std::vector<std::string> tokenList;
    void tokenize(std::string &expression);
    // static const std::vector<std::string> operator;

   public:
    Tokenizer(std::string &);
    ~Tokenizer();

    std::vector<std::string> getTokenList() const;
};

// const std::vector<std::string> Tokenizer::operator({"+", "-", "*", "/"});

#endif
