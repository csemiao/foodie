//
// Created by Chris on 2018-02-01.
//

#include "Token.h"
#include "utils/Logger.h"

Token::Token(TokenType type, std::string token) :
    m_tokenType(type),
    m_token(token)
{
    dbPrintf("Token created: '%s'", m_token.c_str());
}

Token::Token() :
        m_tokenType(Token::DEFAULT),
        m_token("")
{

}

Token::TokenType Token::type() {
    return m_tokenType;
}

