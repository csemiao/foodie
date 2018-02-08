//
// Created by Chris on 2018-01-31.
//

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <map>

#include "Tokenizer.h"
#include "Token.h"
#include "utils/Logger.h"



using namespace std;

namespace
{
    std::map<std::string, Token::TokenType> keywords =
        {
                {"ingredients", Token::VARIABLE_ASSIGNMENT_START},
                {"preparation", Token::PROCEDURE_START},
                {"combine", Token::ADD_SELF},
                {"mix", Token::MULTIPLY_SELF},
                {"fold", Token::NEGATION},
                {"stir", Token::RECIPROCAL},
                {"serve", Token::END_OF_FUNCTION}
        };

    // If a token is available from the keyword, create it and return true.  otherwise return false;

    bool tokenFromKeyword(std::string& keyword, Token& token)
    {
        auto idx = keywords.find(keyword);
        if (idx != keywords.end())
        {
            Token::TokenType type = idx->second;
            token = Token(type, keyword);
            return true;
        }
        else
        {
            return false;
        }
    }
}

Tokenizer::Tokenizer() :
    m_line(1),
    m_start(0),
    m_current(0),
    m_sourceLength(0)
{
}

void Tokenizer::reset(shared_ptr<string> input)
{
    m_line = 1;
    m_start = 0;
    m_current = 0;
    m_source = input;
    m_sourceLength = input->size();
}


vector<Token> Tokenizer::tokenize(shared_ptr<string> input)
{
    reset(input);

    vector<Token> tokens;

    while (!isFinished())
    {
        Token token;
        if(scanForNextToken(token))
        {
            tokens.push_back(token);
        }
        else
        {
            // TODO: What should go here?
        }
    }

    Token endToken(Token::END_OF_FILE, "END_OF_FILE");
    tokens.push_back(endToken);


    return tokens;
}


bool Tokenizer::isFinished()
{
    return (m_current > m_sourceLength - 1);
}

bool Tokenizer::scanForNextToken(Token& tokenOut)
{
    while (peekSource()) {
        char c = peekSource();
        switch (c)
        {
            case '.':
            {
                popSource();
                tokenOut = Token(Token::END_OF_STATEMENT, ".");
                return true;
            }
            default: {
                if (isdigit(c)) {
                    tokenOut = findNumber();
                    return true;
                }
                else if (isalpha(c)) {
                    tokenOut = findWord();
                    return true;
                }
                else
                {
                    popSource();
                    break;
                }

            }
        }
    }

    // TODO: What should this return if it finds nothing????
}

char Tokenizer::popSource()
{
    if (isFinished())
    {
        return '\0';
    }

    m_current++;
    return m_source->at(m_current - 1);
}

char Tokenizer::peekSource()
{
    if (isFinished())
    {
        return '\0';
    }

    return m_source->at(m_current);
}

Token Tokenizer::findNumber()
{
    m_start = m_current;

    bool isFloat = false;

    while (peekSource() && peekSource() != ' ')
    {
        char c = popSource();
        if (isalpha(c))
        {
            // TODO maybe this could be treated as as word instead?
            ParseError("Invalid number %s", "the substring");     // TODO substring
        }
        if (c == '.')
        {
            if (!isFloat)
            {
                isFloat = true;
            }
            else
            {
                ParseError("Invalid number %s", "a substring");   // TODO substring
            }
        }
    }

    if (isFloat)
    {
        return Token(Token::FLOAT, m_source->substr(m_start, m_current - m_start));
    }
    else
    {
        return Token(Token::INTEGER, m_source->substr(m_start, m_current - m_start));
    }
}

Token Tokenizer::findWord()
{
    m_start = m_current;

    while (peekSource() && peekSource() != ' ' && peekSource() != '.' && peekSource() != '\n')
    {
        popSource();
    }

    Token token;
    std::string word = m_source->substr(m_start, m_current - m_start);

    if (tokenFromKeyword(word, token))
    {
        return token;
    }
    else
    {
        return Token(Token::UNKNOWN, word);
    }
}

void Tokenizer::ParseError(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    parseErrorPrintf(m_line, m_current, format, args);
    va_end(args);
}

