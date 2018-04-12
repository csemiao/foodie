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
        WORD,                // something that is not anything else.  most likely a string

        // Declaration types
        FUNCTION_DECLARATION,
        TRANSFER_VARIABLE,
        INGREDIENTS_START,
        PROCEDURE_START,
        TRANSFER_VALUE,
        BRAND_ID,
        FUNCTION_CALL,
        FUNCTION_ARG,
        FUNCTION_ARGS_START,

        // Numerical and Logical types
        INTEGER,
        FLOAT,
        // BOOLEAN,

        // Primitive Mathematical Operation
        ADD_SELF,
        MULTIPLY_SELF,
        NEGATION,
        RECIPROCAL,

        // Boolean Operations
        // AND,
        // OR,
        // NOT,
        // EQUALS,
        // NOT_EQUALS,

        // Control flow
        // IF,
        // ELSEIF,
        // ELSE,
        // FOR,
        // WHILE,

        // Endings
        END_OF_STATEMENT,
        RETURN,
        END_OF_FILE,

        // Functions
        PRINT
    };

public:
    friend bool operator==(const Token& left, const Token& right);

    Token(TokenType type, std::string token);
    Token();

    TokenType type() const;
    std::string token() const;

private:
    TokenType       m_tokenType;
    std::string     m_token;
};