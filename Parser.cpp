//
// Created by Chris on 2018-02-07.
//


#include <vector>
#include "Parser.h"
#include "Token.h"
#include "Expression.h"


using namespace std;

Parser::Parser() :
    m_pos(0),
    m_length(0),
    m_start(0),
    m_tokens()
{

}


void Parser::parse(vector<Token> tokens)
{
    m_tokens = tokens;
    while (!isAtEnd())
    {
        Expression exp = FindNextExpression();
    }
}

Expression Parser::FindNextExpression()
{
    enum State
    {
        FUNCTION_DECLARATION,
        VARIABLES,
        EXPRESSIONS,
    };

    State state = FUNCTION_DECLARATION;

    while (!isAtEnd())
    {
        Token t;
        if (popToken(t))
        {
            switch (t.type())
            {

            }
        }
    }

    return Expression();
}

bool Parser::isAtEnd()
{
    return (m_pos > m_tokens.size() - 1);
}

bool Parser::peekToken(Token& outToken)
{
    if (!isAtEnd())
    {
        outToken = m_tokens.at(m_pos);
        return true;
    }
    return false;
}

bool Parser::popToken(Token& outToken)
{
    if (!isAtEnd())
    {
        m_pos++;
        outToken = m_tokens.at(m_pos - 1);
        return true;
    }
    return false;
}
