//
// Created by Chris on 2018-02-24.
//

#include "Interpreter.h"
#include "utils/Logger.h"
#include <boost/variant/get.hpp>
#include <string>
#include <map>
#include <stack>
#include <memory>

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

FoodieFunction::FoodieFunction(std::string name) :
    m_name(name),
    m_arity(0)
{};

void FoodieFunction::addStatement(std::shared_ptr<Statement> statement)
{
    m_statements.push_back(statement);
}

void FoodieFunction::addArg(std::string arg)
{
    for (std::map<int, std::string>::iterator it = m_args.begin(); it != m_args.end(); ++it)
    {
        auto pair = *it;
        if (pair.second == arg)
        {
            fatalPrintf("The food from the recipe for \"%s\" is already an ingredient", arg.c_str());
        }
    }

    m_args[m_arity] = arg;
    m_arity += 1;
}

void FoodieFunction::call(std::map<std::string, variant>& args, Interpreter& interpreter)
{
    // Set up a new environment using our args
    interpreter.m_environment.push(std::make_unique<std::map<std::string, variant>>(args));
    interpreter.interpret(m_statements);
};


template <class T>
variant Interpreter::doAddition(std::vector<Literal>& source, Literal& target)
{
    std::map<std::string, variant> localEnv = *m_environment.top();
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
                m_environment.top()->operator[](lit.m_token.token()) = 0;
                break;
            }
            case 1:
            {
                resultVal += srcVal;
                m_environment.top()->operator[](lit.m_token.token()) = 0.0f;
                break;
            }
            case 2:
            {
                resultVal = resultVal + srcVal;     // This is intentional based on cooking semantics.
                m_environment.top()->operator[](lit.m_token.token()) = "";
                break;
            }
            default:
            {
                fatalPrintf("operation + variant case not accounted for %d", src.which());
            }
        }
    }

    m_environment.top()->operator[](target.m_token.token()) = variant(resultVal);
    return lookup(target.m_token.token());
}

template<class T>
variant Interpreter::doMultiplication(std::vector<Literal>& source, Literal& target)
{
    std::map<std::string, variant> localEnv = *m_environment.top();
    variant result = lookup(target.m_token.token());
    T resultVal = boost::get<T>(result);

    for (Literal lit : source)
    {
        variant src = lookup(lit.m_token.token());
        T srcVal = boost::get<T>(src);
        resultVal *= srcVal;

        m_environment.top()->operator[](lit.m_token.token()) = (T)0;
    }

    m_environment.top()->operator[](target.m_token.token()) = variant(resultVal);

    return lookup(target.m_token.token());
}

template <class T>
variant Interpreter::doNegation(Literal& target)
{
    std::map<std::string, variant> localEnv = *m_environment.top();
    variant tgt = lookup(target.m_token.token());
    T tgtVal = boost::get<T>(tgt);

    m_environment.top()->operator[](target.m_token.token()) = (-tgtVal);

    return lookup(target.m_token.token());
}

template <class T>
variant Interpreter::doReciprocal(Literal& target)
{
    std::map<std::string, variant> localEnv = *m_environment.top();
    variant tgt = lookup(target.m_token.token());
    T tgtVal = boost::get<T>(tgt);

    switch(tgt.which())
    {
        case 0:
        case 1:
        {
            m_environment.top()->operator[](target.m_token.token()) = (1/tgtVal);
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
        case Token::WORD:
        {
            value = expression.m_token.token();
            break;
        }
        default:
        {
            fatalPrintf("Unknown primitive type being assigned %s", expression.m_token.token().c_str());
        }
    }

    m_environment.top()->operator[](name) = value;
}

Interpreter::Interpreter() :
        StatementVisitor()
{
    m_environment.push(std::make_unique<std::map<std::string, variant>>());
}

void Interpreter::interpret(std::vector<std::shared_ptr<Statement>>& statements)
{

    for (std::shared_ptr<Statement> s: statements)
    {
        // This is a huge kludge.  But we don't actually visit the type until the statement is read.
        ReturnStatement* prs = dynamic_cast<ReturnStatement*>(s.get());
        FunctionArgStatement* fas = dynamic_cast<FunctionArgStatement*>(s.get());

        // TODO: refactor.  this is turning into a whole mess of spaghetti.
        // TODO: maybe make a first pass of everything and process functiondecs, functionargs first
        if (activeFunction != NONE)
        {
            if (fas != nullptr)
            {
                (*s).accept(*this);
            }
            else
            {
                m_functions[activeFunction]->addStatement(s);
            }
        }
        else
        {
            (*s).accept(*this);
        }

        if (prs != nullptr)
        {
            activeFunction = NONE;
        }
    }
}

variant Interpreter::lookup(std::string key)
{
    std::map<std::string, variant> localEnv = *m_environment.top();
    if (localEnv.count(key) == 1)
    {
        return localEnv.at(key);
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

    std::map<std::string, variant> localEnv = *m_environment.top();

    if (localEnv.count(name) == 0)
    {
        doAssignment(name, *statement.mp_expression);
        std::map<std::string, variant> localEnv2 = *m_environment.top();
    }
    else
    {
        // If an ingredient is already listed, then it's a mistake for it to be listed again
        fatalPrintf("%s is already in the ingredient list", name.c_str());
    }
}

void Interpreter::visitFunctionDecStatement(FunctionDecStatement& statement)
{
    if (activeFunction == NONE && m_functions.count(statement.m_name) == 0)
    {
        m_functions[statement.m_name] = std::make_unique<FoodieFunction>(statement.m_name);
        activeFunction = statement.m_name;
    }
    else if (activeFunction != NONE)
    {
        fatalPrintf("You can't put recipe \"%s\" inside recipe \"%s\", use a new cookbook page", statement.m_name.c_str(), activeFunction.c_str());
    }
    else if (m_functions.count(statement.m_name) != 0)
    {
        fatalPrintf("Your cookbook already has a recipe for \"%s\" in it", statement.m_name.c_str());
    }
}

void Interpreter::visitFunctionArgStatement(FunctionArgStatement& statement)
{
    if (activeFunction != NONE)
    {
        m_functions[activeFunction]->addArg(statement.m_name);
    }
    else
    {
        fatalPrintf("You need a recipe for \"%s\", before you can use it as an ingredient", statement.m_name.c_str());
    }
}

void Interpreter::visitFunctionCallStatement(FunctionCallStatement &statement)
{
    if (m_functions.count(statement.m_name) == 0)
    {
        fatalPrintf("Couldn't find a recipe for \"%s\" to use", statement.m_name.c_str());
    }

    FoodieFunction function = *m_functions[statement.m_name];

    if (function.m_arity != statement.m_args.size())
    {
        fatalPrintf("You don't have the right number of ingredients to make \"%s\"", statement.m_name.c_str());
    }

    // Use the function arguments as a starter environment
    std::map<std::string, variant> funcArgs = getFunctionArgsFromEnvironment(statement.m_args);
    // And call the function
    function.call(funcArgs, *this);
}

std::map<std::string, variant> Interpreter::getFunctionArgsFromEnvironment(std::vector<std::string> args)
{
    std::map<std::string, variant> argsMap;

    for (std::string s : args)
    {
        variant value = lookup(s);
        argsMap[s] = value;
    }

    return argsMap;
};

void Interpreter::visitReturnStatement(ReturnStatement &statement)
{
    std::map<std::string, variant> localEnv = *m_environment.top();

    activeFunction = NONE;

    if (statement.m_name != NONE && localEnv.count(statement.m_name) == 0)
    {
        fatalPrintf ("You didn't have \"%s\", so you can't serve it", statement.m_name.c_str());
    }
    else
    {
        bool hasReturn = false;
        variant returnValue;

        if (statement.m_name != NONE)
        {
            returnValue = localEnv[statement.m_name];
            hasReturn = true;
        }

        if (m_environment.size() > 1)
        {
            // If we can, put the return value into the stack frame below
            m_environment.pop();
            if (hasReturn)
            {
                m_environment.top()->operator[](statement.m_name) = returnValue;
            }
        }
    }
}

variant Interpreter::evaluate(std::shared_ptr<Expression> expression)
{
    return expression->evaluate(*this);
}

/**
 * Expression Visitor
 */

variant Interpreter::visitBinary(Binary& binary)
{
    if (!isAllSameType(binary.m_source, binary.m_target))
    {
        dbPrintf("Invalid types specified: %s", "Refactor me into a better method");
    }

    switch (binary.m_op.type())
    {
        case Token::ADD_SELF:
        {
            AdditionVisitor visitor(binary, *this);
            return boost::apply_visitor(visitor, lookup(binary.m_source.at(0).m_token.token()));
        }
        case Token::MULTIPLY_SELF:
        {
            MultiplicationVisitor visitor(binary, *this);
            return boost::apply_visitor(visitor, lookup(binary.m_source.at(0).m_token.token()));
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





