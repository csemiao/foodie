//
// Created by Chris on 2018-02-25.
//

#pragma once
#include "Expression.h"

// Forward Declares
class ExpressionStatement;
class PrintStatement;
class AssignmentStatement;
class FunctionDecStatement;
class FunctionArgStatement;
class FunctionCallStatement;
class ReturnStatement;


// Visitor
class StatementVisitor
{
public:
    virtual variant visitExpressionStatement(ExpressionStatement& statement) = 0;
    virtual void visitPrintStatement(PrintStatement& statement) = 0;
    virtual void visitAssignmentStatement(AssignmentStatement& statement) = 0;
    virtual void visitFunctionDecStatement(FunctionDecStatement& statement) = 0;
    virtual void visitFunctionArgStatement(FunctionArgStatement& statement) = 0;
    virtual void visitFunctionCallStatement(FunctionCallStatement& statement) = 0;
    virtual void visitReturnStatement(ReturnStatement& statement) = 0;
};


// Statement
class Statement {
public:
    virtual ~Statement();
    virtual void accept(StatementVisitor& visitor) = 0;
};

class FunctionDecStatement : public Statement
{
public:
    FunctionDecStatement(std::string name);
    void accept(StatementVisitor& visitor);

    std::string m_name;
};

class FunctionArgStatement : public Statement
{
public:
    FunctionArgStatement(std::string name);
    void accept(StatementVisitor& visitor);

    std::string m_name;
};

class ReturnStatement : public Statement
{
public:
    ReturnStatement(std::string name);
    void accept(StatementVisitor& visitor);

    std::string m_name;
};


class FunctionCallStatement : public Statement
{
public:
    FunctionCallStatement(std::string name, std::vector<std::string> args);
    void accept(StatementVisitor& visitor);

    std::string m_name;
    std::vector<std::string> m_args;
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
