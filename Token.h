//
// Created by Chris on 2018-01-31.
//

#pragma once

#include <string>

class Token
{
public:
    enum TokenType
    {
        // lexical sugar that's there just for natural language style
        DEFAULT = -1,
        UNKNOWN,

        // Declaration types
        FUNCTION_DECLARATION,
        VARIABLE_ASSIGNMENT,
        VARIABLE_ASSIGNMENT_START,
        PROCEDURE_START,

        // Primitive type
        INTEGER,
        FLOAT,
        BOOLEAN,
        STRING,

        // Primitive Mathematical Operation
        ADD_SELF,
        MULTIPLY_SELF,
        NEGATION,
        RECIPROCAL,

        // Boolean Operations
        AND,
        OR,
        NOT,
        EQUALS,
        NOT_EQUALS,

        // Control flow
        IF,
        ELSEIF,
        ELSE,
        FOR,
        WHILE,

        // Endings
        END_OF_STATEMENT,
        END_OF_FUNCTION,
        END_OF_FILE,
    };

public:
    Token(TokenType type, std::string token);
    Token();

    TokenType type();

private:
    TokenType       m_tokenType;
    std::string     m_token;
};