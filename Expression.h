//
// Created by Chris on 2018-02-07.
//

#include "Token.h"
#include <string>
#include <vector>
#include <boost/variant/variant.hpp>
#pragma once

using variant = boost::variant<int,float,std::string>;

// Forward declares
class Binary;
class Literal;
class Unary;

// Visitor
class ExpressionVisitor
{
public:
    virtual variant visitBinary(Binary& binary) = 0;
    virtual variant visitLiteral(Literal& literal) = 0;
    virtual variant visitUnary(Unary& unary) = 0;
};


class Expression
{
public:
    virtual ~Expression();
    virtual variant evaluate(ExpressionVisitor& visitor) = 0;
};


class Literal : public Expression {
public:
    Literal(Token token);

    Token::TokenType type();
    variant evaluate(ExpressionVisitor &visitor);

    Token m_token;
};

class Binary : public Expression
{
public:
    Binary(std::vector<Literal> sources, Token op, Token transferType, Literal target);
    variant evaluate(ExpressionVisitor& visitor);

    std::vector<Literal> m_source;
    Literal m_target;
    const Token m_op;
    const Token m_transferType;
};

class Unary : public Expression
{
public:
    Unary(Literal source, Token op);
    variant evaluate(ExpressionVisitor& visitor);

    Literal m_source;
    const Token m_op;
};






