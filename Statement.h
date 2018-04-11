//
// Created by Chris on 2018-02-25.
//

#pragma once
#include "Expression.h"

// Forward Declares
class ExpressionStatement;
class PrintStatement;
class AssignmentStatement;


// Visitor
class StatementVisitor
{
public:
    virtual variant visitExpressionStatement(ExpressionStatement& statement) = 0;
    virtual void visitPrintStatement(PrintStatement& statement) = 0;
    virtual void visitAssignmentStatement(AssignmentStatement& statement) = 0;
};


// Statement
class Statement {
public:
    virtual ~Statement();
    virtual void accept(StatementVisitor& visitor) = 0;
};

// ExpressionStatement
class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(std::shared_ptr<Expression> expression);
    void accept(StatementVisitor& visitor);

    std::shared_ptr<Expression> mp_expression;
};

// PrintStatement
class PrintStatement : public Statement
{
public:
    PrintStatement(std::shared_ptr<Literal> literal);
    void accept(StatementVisitor& visitor);

    std::shared_ptr<Literal> mp_expression;
};

// AssignmentStatement
class AssignmentStatement: public Statement
{
public:
    AssignmentStatement(std::shared_ptr<Literal> name, std::shared_ptr<Literal> expression);

    void accept(StatementVisitor& visitor);
    std::shared_ptr<Literal> mp_name;
    std::shared_ptr<Literal> mp_expression;
};
