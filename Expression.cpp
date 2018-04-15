//
// Created by Chris on 2018-02-07.
//

#include "Expression.h"


/**
 * EXPRESSION
 */

Expression::~Expression() {};

/**
 * LITERAL
 */

Literal::Literal(Token token) :
        Expression()
{
    m_token = token;
};

Token::TokenType Literal::type()
{
    return m_token.type();
}

variant Literal::evaluate(ExpressionVisitor& visitor)
{
    return visitor.visitLiteral(*this);
}


/**
 * BINARY
 */

Binary::Binary(std::vector<Literal> input, Token op, Token::TokenType transferType, Literal assignTo) :
        Expression(),
        m_source(input),
        m_target(assignTo),
        m_transferType(transferType),
        m_op(op)
{};

variant Binary::evaluate(ExpressionVisitor& visitor)
{
    return visitor.visitBinary(*this);
};

/**
 * UNARY
 */

Unary::Unary(Literal source, Token op) :
    Expression(),
    m_source(source),
    m_op(op)
{};

variant Unary::evaluate(ExpressionVisitor& visitor)
{
    return visitor.visitUnary(*this);
}