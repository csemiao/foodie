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
        void reset();
        std::vector<std::shared_ptr<Statement>> parse(std::vector<Token>& tokens);

    private:

        std::shared_ptr<Statement> nextStatement();
        std::shared_ptr<Statement> makeStatement(std::vector<Token>& tokens);

        std::shared_ptr<ExpressionStatement> makeBinary(std::vector<Token>& tokens);
        std::shared_ptr<ExpressionStatement> makeUnary(std::vector<Token>& tokens);
        std::shared_ptr<PrintStatement> makePrint(std::vector<Token>& tokens);
        std::shared_ptr<AssignmentStatement> makeAssignment(std::vector<Token>& tokens);

        void parseError(std::vector<Token> tokens, std::string msg);

        bool isAtEnd();
        bool popToken(Token& outToken);

        int m_pos;
        int m_length;
        int m_start;
        std::vector<Token> m_tokens;
};
