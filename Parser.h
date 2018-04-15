#pragma once

#include <vector>
#include <memory>

#include "Token.h"
#include "Expression.h"
#include "Statement.h"


class Parser
{
    public:
        Parser();
        std::vector<std::shared_ptr<Statement>> parse(std::vector<Token>& tokens);

    private:

        bool nextStatement(std::shared_ptr<Statement>& p_statement);
        Statement* makeStatement(std::vector<Token>& tokens);

        FunctionDecStatement* makeFunctionDec(std::vector<Token>& tokens);
        ExpressionStatement* makeBinary(std::vector<Token>& tokens);
        ExpressionStatement* makeUnary(std::vector<Token>& tokens);
        PrintStatement* makePrint(std::vector<Token>& tokens);
        AssignmentStatement* makeAssignment(std::vector<Token>& tokens);
        FunctionArgStatement* makeFunctionArg(std::vector<Token>& tokens);
        FunctionCallStatement* makeFunctionCall(std::vector<Token>& tokens);
        ReturnStatement* makeReturn(std::vector<Token>& tokens);


        void parseError(std::vector<Token> tokens, std::string msg);

        bool isAtEnd();
        bool popToken(Token& outToken);
        bool peekToken(Token& outToken);

        int m_pos;
        int m_length;
        int m_start;
        std::vector<Token> m_tokens;
};
