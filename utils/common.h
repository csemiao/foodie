#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "../Token.h"

std::string tokenVectorToString(std::vector<Token> vec, int start = 0, bool spaces = false)
{
    std::stringstream ss;

    for (std::vector<Token>::iterator it = vec.begin() + start; it != vec.end(); ++it)
    {
        Token t = *it;

        ss << t.token();

        if (spaces && it != vec.end() -1)
        {
            ss << " ";
        }
    }

    return ss.str();
}