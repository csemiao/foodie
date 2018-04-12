//
// Created by Chris on 2018-02-24.
//

#pragma once

#include "Expression.h"
#include "Statement.h"
#include "utils/logger.h"
#include <boost/variant/variant.hpp>
#include <map>

using variant = boost::variant<int,float,std::string>;

class NegationVisitor;
class ReciprocalVisitor;


struct FoodieFunction
{
    FoodieFunction(std::string name);
    void call(std::vector<Literal> args);

    void addStatement(std::shared_ptr<Statement> statement);
    void addArg(std::string);

private:
    std::string m_name;
    int m_arity;
    std::map<int, std::string> m_args;      // 0-indexed.
    std::vector<std::shared_ptr<Statement>> m_statements;
};


class Interpreter : private StatementVisitor, private ExpressionVisitor
{
    friend class NegationVisitor;
    friend class ReciprocalVisitor;

public:
    Interpreter();
    void interpret(std::vector<std::shared_ptr<Statement>>& statements);
    variant evaluate(std::shared_ptr<Expression> expression);
    variant evaluate(Expression& expression);

private:
    variant lookup(std::string key);
    void doPrint(variant& result, std::string name);
    void doAssignment(std::string& name, Literal& value);

    template <class T>
    variant doAddition(std::vector<Literal>& sources, Literal& target);
    template <class T>
    variant doMultiplication(std::vector<Literal>& sources, Literal& target);
    template <class T>
    variant doNegation(Literal& target);
    template <class T>
    variant doReciprocal(Literal& target);

    std::map<std::string, variant> m_variables;
    std::map<std::string, std::unique_ptr<FoodieFunction>> m_functions;

// For StatementVisitor
private:
    variant visitExpressionStatement(ExpressionStatement& statement);
    void visitPrintStatement(PrintStatement& statement);
    void visitAssignmentStatement(AssignmentStatement& statement);
    void visitFunctionDecStatement(FunctionDecStatement& statement);
    void visitFunctionArgStatement(FunctionArgStatement& statement);

// For ExpressionVisitor
private:
    variant visitLiteral(Literal& literal);
    variant visitBinary(Binary& binary);
    variant visitUnary(Unary& unary);
};


struct ReciprocalVisitor : public boost::static_visitor<variant>
{
    ReciprocalVisitor(Unary unary, Interpreter& interpreter) :
    m_expression(unary),
    m_interpreter(interpreter)
    {};

    template<class T>
    variant operator() (const T& operand)
    {
        return m_interpreter.doReciprocal<T>(m_expression.m_source);
    }

    variant operator() (const std::string& operand)
    {
        fatalPrintf("I'm printing from within a template %s", "yay");
    };

    Unary m_expression;
    Interpreter& m_interpreter;
};


struct NegationVisitor : public boost::static_visitor<variant>
{
    NegationVisitor(Unary unary, Interpreter& interpreter) :
            m_expression(unary),
            m_interpreter(interpreter)
    {};

    template<class T>
    variant operator() (const T& operand)
    {
        return m_interpreter.doNegation<T>(m_expression.m_source);
    }

    variant operator() (const std::string& operand)
    {
        fatalPrintf("I'm printing from within a template %s", "yay");
    };

    Unary m_expression;
    Interpreter& m_interpreter;
};







