//
// Created by Chris on 2018-02-24.
//

#pragma once

#include "Expression.h"
#include "Statement.h"
#include <boost/variant/variant.hpp>
#include <map>

using variant = boost::variant<int,float,std::string>;

class Interpreter : private StatementVisitor, private ExpressionVisitor
{
public:
    Interpreter();
    void interpret(std::vector<std::shared_ptr<Statement>>& statements);
    variant evaluate(std::shared_ptr<Expression> expression);

private:
    variant lookup(std::string key);
    void doPrint(variant& result, std::string name);
    template <class T>
    variant doAddition(std::vector<Literal>& sources, Literal& target);
    template <class T>
    variant doMultiplication(std::vector<Literal>& sources, Literal& target);
    template <class T>
    variant doNegation(Literal& target);
    template <class T>
    variant doReciprocal(Literal& target);

    std::map<std::string, variant> m_variables;

// For StatementVisitor
private:
    variant visitExpressionStatement(ExpressionStatement& statement);
    void visitPrintStatement(PrintStatement& statement);

// For ExpressionVisitor
private:
    variant visitLiteral(Literal& literal);
    variant visitBinary(Binary& binary);
    variant visitUnary(Unary& unary);
};







