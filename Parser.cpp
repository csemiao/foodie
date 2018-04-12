//
// Created by Chris on 2018-02-07.
//

#include "utils/Logger.h"
#include "utils/common.h"

#include <vector>
#include "Parser.h"

namespace
{
    bool isIngredient = false;
    bool isProcedure = true;

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

// TODO:  Refactor to a switch based state??
std::shared_ptr<Statement> Parser::nextStatement()
{
    std::vector<Token> tokens;
    Token nextToken;

    while (popToken(nextToken))
    {
        if (nextToken.type() == Token::FUNCTION_DECLARATION)
        {
            isIngredient = false;
            isProcedure = false;

            // Because I don't want to have a period in the recipe declaration,
            // need to grab all the tokens which make up the function name at this point
            tokens.push_back(nextToken);

            Token ahead;
            while (peekToken(ahead) && ahead.type() != Token::INGREDIENTS_START)
            {
                popToken(nextToken);
                tokens.push_back(nextToken);
            }

            return makeStatement(tokens);
        }
        else if (nextToken.type() == Token::INGREDIENTS_START)
        {
            isIngredient = true;
            isProcedure = false;
            continue;
        }
        else if (nextToken.type() == Token::PROCEDURE_START)
        {
            isProcedure = true;
            isIngredient = false;
            continue;
        }
        else if (nextToken.type() == Token::END_OF_STATEMENT)
        {
            return makeStatement(tokens);
        }

        tokens.push_back(nextToken);
    }
}

// TODO:  Refactor.  We can remove the ingredients flag and just check if it contains an assignment operator
std::shared_ptr<Statement>Parser::makeStatement(std::vector<Token>& tokens)
{
    if (isIngredient)
    {
        switch (tokens.at(0).type())
        {
            case Token::FUNCTION_ARG:
            {
                return makeFunctionArg(tokens);
            }
            default:
            {
                return makeAssignment(tokens);
            }
        }
    }
    else // if (isProcedure)
    {
        switch (tokens.at(0).type())
        {
            case Token::FUNCTION_DECLARATION:
            {
                return makeFunctionDec(tokens);
            }
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
                fatalPrintf("I don't know what do with this token type %d", tokens.at(0).type());
            }
        }
    }
}

std::shared_ptr<FunctionDecStatement> Parser::makeFunctionDec(std::vector<Token>& tokens)
{
    std::stringstream ss;

    // We'll say that function names can be multi-word.
    for (std::vector<Token>::iterator it = tokens.begin() + 1; it != tokens.end(); ++it)
    {
        Token t = *it;

        if (it != tokens.end() - 1)
        {
            ss << t.token() << " ";
        }
    }

    return std::make_shared<FunctionDecStatement>(ss.str());
}


std::shared_ptr<FunctionArgStatement> Parser::makeFunctionArg(std::vector<Token>& tokens)
{
    std::string argName = tokenVectorToString(tokens, 1, true);
    return make_shared<FunctionArgStatement>(argName);
}

std::shared_ptr<AssignmentStatement> Parser::makeAssignment(std::vector<Token>& tokens)
{

    Token value = tokens.at(0);
    std::shared_ptr<Literal> p_value = std::make_shared<Literal>(tokens.at(0));

    switch (tokens.at(0).type())
    {
        case Token::INTEGER:
        case Token::FLOAT:
        {
            // TODO: check length = 3 (includes EOS)
            std::shared_ptr<Literal> p_name = std::make_shared<Literal>(tokens.at(1));
            return make_shared<AssignmentStatement>(p_name, p_value);
        }
        case Token::WORD:
        {
            // TODO: check length = 4 (includes EOS)
            if (tokens.at(1).type() == Token::BRAND_ID)
            {
                std::shared_ptr<Literal> p_name = std::make_shared<Literal>(tokens.at(2));
                return make_shared<AssignmentStatement>(p_name, p_value);
            }
            else
            {
                // TODO:  We need to know what brand type
                break;
            }
        }
        default:
        {
            fatalPrintf("I don't know what do do with this primitive type %d", tokens.at(0).type());
        }
    }
    //the first token is the value
    //the second token is the name, except if it's brand, in which case make it
}

std::shared_ptr<PrintStatement> Parser::makePrint(std::vector<Token>& tokens)
{
    //dbPrintf("Trying to make print statement using tokens %s", "");
    if (tokens.size() != 2)
    {
        stringstream ss;
        ss << "Invalid number of instagram photos: ";
        ss << tokens.size() - 1;
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
    if (tokens.size() != 2)
    {
        stringstream ss;
        ss << "Invalid number of parameters to operation";
        ss << tokens.size() - 1;
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

bool Parser::peekToken(Token& outToken)
{
    if (!isAtEnd())
    {
        outToken = m_tokens.at(m_pos);
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



