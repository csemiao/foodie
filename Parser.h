//
// Created by Chris on 2018-02-07.
//

#pragma once

#include <vector>
#include "Token.h"
#include "Expression.h"

class Parser {
public:
    Parser();
    void reset();
    void parse(std::vector<Token> tokens);

private:

    Expression FindNextExpression();

    bool isAtEnd();
    bool peekToken(Token& outToken);
    bool popToken(Token& outToken);

    int m_pos;
    int m_length;
    int m_start;
    std::vector<Token> m_tokens;
    std::vector<std::vector<Token>> m_expressions;
};
