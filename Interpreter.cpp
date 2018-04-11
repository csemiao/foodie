//
// Created by Chris on 2018-02-24.
//

#include "Interpreter.h"
#include "utils/Logger.h"
#include <boost/variant/get.hpp>
#include <string>

namespace
{
    std::vector<std::string> lineEndings {"fantastic", "yummy", "scrumptious", "delicious", "like crap",
                                           "tasty", "weird", "off-color", "strange",
                                            "disgusting", "passable", "edible", "like dog poop"};

    bool isAllSameTypeVec(std::vector<Literal>& literals)
    {
        for (Literal& lit : literals)
        {
            if (lit.type() != literals.at(0).type())
            {
                return false;
            }
        }

        return true;
    }

    bool isAllSameType(std::vector<Literal>& literals, Literal& target) {

        return isAllSameTypeVec(literals) && target.type() == literals.at(0).type();
    }
}

template <class T>
variant Interpreter::doAddition(std::vector<Literal>& source, Literal& target)
{
    variant result = lookup(target.m_token.token());
    T resultVal = boost::get<T>(result);

    for (Literal lit : source)
    {
        variant src = lookup(lit.m_token.token());
        T srcVal = boost::get<T>(src);

        switch (src.which())
        {
            case 0:
            {
                resultVal += srcVal;
                m_variables[lit.m_token.token()] = 0;
                break;
            }
            case 1:
            {
                resultVal += srcVal;
                m_variables[lit.m_token.token()] = 0.0f;
                break;
            }
            case 2:
            {
                resultVal = resultVal + srcVal;
                m_variables[lit.m_token.token()] = "";
                break;
            }
            default:
            {
                fatalPrintf("operation + variant case not accounted for %d", src.which());
            }
        }
    }

    m_variables[target.m_token.token()] = variant(resultVal);
    return lookup(target.m_token.token());
}

template<class T>
variant Interpreter::doMultiplication(std::vector<Literal>& source, Literal& target)
{
    variant result = lookup(target.m_token.token());
    T resultVal = boost::get<T>(result);

    for (Literal lit : source)
    {
        variant src = lookup(lit.m_token.token());
        T srcVal = boost::get<T>(src);

        switch (src.which())
        {
            case 0:
            {
                resultVal *= srcVal;
                m_variables[lit.m_token.token()] = 0;
                break;
            }
            case 1:
            {
                resultVal *= srcVal;
                m_variables[lit.m_token.token()] = 0.0f;
                break;
            }
            case 2:
            {
                fatalPrintf("operation * not supported for type");
                break;
            }
            default:
            {
                fatalPrintf("operation * variant case not accounted for %d", src.which());
            }
        }
    }

    m_variables[target.m_token.token()] = variant(resultVal);

    return lookup(target.m_token.token());
}

template <class T>
variant Interpreter::doNegation(Literal& target)
{
    variant tgt = lookup(target.m_token.token());
    T tgtVal = boost::get<T>(tgt);

    m_variables[target.m_token.token()] = (-tgtVal);

    return lookup(target.m_token.token());
}

template <class T>
variant Interpreter::doReciprocal(Literal& target)
{
    variant tgt = lookup(target.m_token.token());
    T tgtVal = boost::get<T>(tgt);

    switch(tgt.which())
    {
        case 0:
        case 1:
        {
            m_variables[target.m_token.token()] = (1/tgtVal);
            break;
        }
        case 2:
        {
            fatalPrintf("operation recip not supported for %s", target.m_token.token().c_str());
            break;
        }
        default:
        {
            fatalPrintf("operation recip variant case not accounted for %d", tgt.which());
        }
    }

    return lookup(target.m_token.token());
}

void Interpreter::doPrint(variant& result, std::string name)
{
    int random = rand() % lineEndings.size();
    std::string lineEnding = lineEndings[random];

    switch(result.which())
    {
        case 0:
        {
            printf("Insta that %s: %d, it looks %s\n", name.c_str(), boost::get<int>(result), lineEnding.c_str());
            break;
        }
        case 1:
        {
            printf("Insta that %s: %f, it looks %s\n", name.c_str(), boost::get<float>(result), lineEnding.c_str());
            break;
        }
        case 2:
        {
            printf("Insta that %s: \"%s\", it looks %s\n", name.c_str(), boost::get<std::string>(result).c_str(), lineEnding.c_str());
            break;
        }
        default:
        {
            fatalPrintf("Instagramming unknown primitive type");
        }
    }
}

void Interpreter::doAssignment(std::string& name, Literal& expression)
{
    variant value;

    switch (expression.type())
    {
        case Token::INTEGER:
        {
            value = stoi(expression.m_token.token());
            break;
        }
        case Token::FLOAT:
        {
            value = stof(expression.m_token.token());
            break;
        }
        case Token::UNKNOWN:
        {
            value = expression.m_token.token();
            break;
        }
        default:
        {
            fatalPrintf("Unknown primitive type being assigned %s", expression.m_token.token().c_str());
        }
    }


    m_variables[name] = value;
}

Interpreter::Interpreter() :
        StatementVisitor()
{
    //m_variables["eggs"] = variant(1.0f);           // TODO: debugging values
    //m_variables["cinammon"] = variant(2.0f);
    //m_variables["toast"] = variant(3.0f);
}

void Interpreter::interpret(std::vector<std::shared_ptr<Statement>>& statements)
{
    for (std::shared_ptr<Statement> s: statements)
    {
        (*s).accept(*this);
    }
}

variant Interpreter::lookup(std::string key)
{
    if (m_variables.count(key) == 1)
    {
        return m_variables.at(key);
    }
    else
    {
        fatalPrintf("Unknown ingredient: %s", key.c_str());
    }
}

/**
 * Statement Visitor
 */


variant Interpreter::visitExpressionStatement(ExpressionStatement& statement)
{
    return evaluate(statement.mp_expression);
}

void Interpreter::visitPrintStatement(PrintStatement& statement)
{
    variant result = evaluate(statement.mp_expression);
    std::string name = statement.mp_expression.get()->m_token.token();
    doPrint(result, name);
}

void Interpreter::visitAssignmentStatement(AssignmentStatement& statement)
{
    std::string name = statement.mp_name.get()->m_token.token();

    if (m_variables.count(name) == 0)
    {
        doAssignment(name, *statement.mp_expression);
    }
    else
    {
        // If an ingredient is already listed, then it's a mistake for it to be listed again
        fatalPrintf("%s is already in the ingredient list", name.c_str());
    }
}

// TODO:  this one can be removed???
variant Interpreter::evaluate(std::shared_ptr<Expression> expression)
{
    return expression->evaluate(*this);
}

variant Interpreter::evaluate(Expression& expression)
{
    return expression.evaluate(*this);
}


/**
 * Expression Visitor
 */

variant Interpreter::visitBinary(Binary& binary)
{
    //dbPrintf("I am visiting a binary expression %s", "");

    if (!isAllSameType(binary.m_source, binary.m_target))
    {
        dbPrintf("Invalid types specified: %s", "Refactor me into a better method");        // TODO: use boosty way instead
    }

    switch (binary.m_op.type())
    {
        case Token::ADD_SELF:
        {
            return Interpreter::doAddition<float>(binary.m_source, binary.m_target);                 // TODO: show why this sucks
        }
        case Token::MULTIPLY_SELF:
        {
            return Interpreter::doMultiplication<float>(binary.m_source, binary.m_target);           // TODO: show why this sucks
        }
        default:
        {
            fatalPrintf("Unknown operation associated to binary expression %d", binary.m_op.type());
        }
    }
}

variant Interpreter::visitLiteral(Literal& literal)
{
    variant result = lookup(literal.m_token.token());
    return result;
}



variant Interpreter::visitUnary(Unary& unary)
{
    //dbPrintf("I am visiting a unary expression %s", "");

    switch (unary.m_op.type())
    {
        case Token::NEGATION:
        {
            NegationVisitor visitor(unary, *this);
            return boost::apply_visitor(visitor, lookup(unary.m_source.m_token.token()));
        }
        case Token::RECIPROCAL:
        {
            ReciprocalVisitor recipVisitor(unary, *this);
            return boost::apply_visitor(recipVisitor, lookup(unary.m_source.m_token.token()));
        }
        default:
        {
            fatalPrintf("Unknown operation associated to unary expression %d", unary.m_op.type());
        }
    }
}
