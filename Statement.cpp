//
// Created by Chris on 2018-02-25.
//

#include "Statement.h"
#include "utils/logger.h"


Statement::~Statement() {};


/**
 * ExpressionStatement
 */
ExpressionStatement::ExpressionStatement(std::shared_ptr<Expression> expression) :
        Statement(),
        mp_expression(expression)
{};

void ExpressionStatement::accept(StatementVisitor& visitor)
{
    visitor.visitExpressionStatement(*this);
}

/**
 * PrintStatement
 */

PrintStatement::PrintStatement(std::shared_ptr<Literal> expression) :
    Statement(),
    mp_expression(expression)
{};

void PrintStatement::accept(StatementVisitor& visitor)
{
    visitor.visitPrintStatement(*this);
}

/**
 * AssignmentStatement
 */

AssignmentStatement::AssignmentStatement(std::shared_ptr<Literal> name, std::shared_ptr<Literal> expression) :
    Statement(),
    mp_name(name),
    mp_expression(expression)
{};

void AssignmentStatement::accept(StatementVisitor& visitor)
{
    visitor.visitAssignmentStatement(*this);
}