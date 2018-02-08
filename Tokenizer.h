//
// Created by Chris on 2018-01-31.
//

#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Token.h"

class Tokenizer
{
public:
    Tokenizer();
    std::vector<Token> tokenize(std::shared_ptr<std::string> input);

private:
    int             m_start;                // start index of the lexeme being considered
    int             m_current;              // current index being considered
    int             m_line;                 // line number
    int             m_sourceLength;         // length of the stuff being parsed
    std::shared_ptr<std::string> m_source;  // the text being parsed

    void    reset(std::shared_ptr<std::string> input);
    bool    scanForNextToken(Token& tokenOut);
    bool    isFinished();
    char    popSource();
    char    peekSource();

    Token   findNumber();
    Token   findWord();

    void    ParseError(const char* format, ...);
};