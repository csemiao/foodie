//
// Created by Chris on 2018-02-07.
//

#include "utils/Logger.h"

#include <vector>
#include "Parser.h"

namespace
{
    size_t findFirstTokenOfType(std::vector<Token>& tokens, Token::TokenType type)
    {
        size_t index = 0;

        for (Token t : tokens)
        {
            if (t.type() == type)
            {
                break;
            }
            index++;
        }

        return index;
    }
}


using namespace std;

Parser::Parser() :
    m_pos(0),
    m_length(0),
    m_start(0),
    m_tokens()
{

}

vector<std::shared_ptr<Statement>> Parser::parse(vector<Token>& tokens)
{
    m_tokens = tokens;
    vector<std::shared_ptr<Statement>> statements;

    while (!isAtEnd())
    {
        std::shared_ptr<Statement> p_statement = nextStatement();
        statements.push_back(p_statement);
    }

    return statements;
}

std::shared_ptr<Statement> Parser::nextStatement()
{
    std::vector<Token> tokens;
    Token nextToken;

    while (popToken(nextToken))
    {
        tokens.push_back(nextToken);

        if (nextToken.type() == Token::END_OF_STATEMENT)
        {
            return makeStatement(tokens);
        }
    }
}

std::shared_ptr<Statement>Parser::makeStatement(std::vector<Token>& tokens)
{
    switch (tokens.at(0).type())
    {
        case Token::ADD_SELF:
        case Token::MULTIPLY_SELF:
        {
            return makeBinary(tokens);
        }
        case Token::NEGATION:
        case Token::RECIPROCAL:
        {
            return makeUnary(tokens);
        }
        case Token::PRINT:
        {
            return makePrint(tokens);
        }
        default:
        {
            dbPrintf("Unknown operation: %s", tokens.at(0).token().c_str());
        }
    }
}

std::shared_ptr<PrintStatement> Parser::makePrint(std::vector<Token>& tokens)
{
    //dbPrintf("Trying to make print statement using tokens %s", "");
    if (tokens.size() != 3)
    {
        stringstream ss;
        ss << "Invalid number of instagram photos: ";
        ss << tokens.size() - 2;
        ss << ", valid number is 1";
        ss << std::endl;
        parseError(tokens, ss.str());
    }

    std::shared_ptr<Literal> literal = std::make_shared<Literal>(tokens.at(1));
    return std::make_shared<PrintStatement>(literal);
}

std::shared_ptr<ExpressionStatement> Parser::makeUnary(std::vector<Token>& tokens)
{
    //dbPrintf("Trying to make unary statement using tokens %s", "");
    if (tokens.size() != 3)
    {
        stringstream ss;
        ss << "Invalid number of parameters to operation";
        ss << tokens.size() - 2;
        ss << ", valid number is 1";
        ss << std::endl;
        parseError(tokens, ss.str());
    }

    std::shared_ptr<Unary> p_unary = std::make_shared<Unary>(tokens.at(1),tokens.at(0));
    return std::make_shared<ExpressionStatement>(p_unary);
}

std::shared_ptr<ExpressionStatement> Parser::makeBinary(std::vector<Token>& tokens)
{
    //dbPrintf("Trying to make a binary statement using tokens %s", "");

    size_t pivot = findFirstTokenOfType(tokens, Token::TRANSFER_VALUE);

    // There should be exactly three tokens after the pivot token, if not then the statement is invalid.
    if (pivot + 3 != tokens.size())
    {
        parseError(tokens, "Invalid statement");
    }

    Token op = tokens.at(0);

    vector<Literal> sources;

    for (int i = 1; i < pivot; i++)
    {
        Token t = tokens.at(i);
        sources.push_back(Literal(t));
    }

    Token target = tokens.at(pivot + 1);

    std::shared_ptr<Binary> p_binary = std::make_shared<Binary>(sources, op, target);

    return std::make_shared<ExpressionStatement>(p_binary);
}

bool Parser::isAtEnd()
{
    return (m_pos > m_tokens.size() - 1);
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


void Parser::parseError(std::vector<Token> tokens, std::string msg)
{
    std::string line;

    for (Token t : tokens)
    {
        line.append(t.token());
        line.append(" ");
    }

    dbPrintf("%s: %s", msg.c_str(), line.c_str());
}


