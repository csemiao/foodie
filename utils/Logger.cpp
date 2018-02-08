//
// Created by Chris on 2018-01-31.
//

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "Logger.h"

void dbPrintfImpl(const char* file, int line, const char* format, ...)
{
    char buff[1024];
    va_list args;
    va_start (args, format);
    snprintf(buff, sizeof(buff), "%s:%d DEBUG: %s\n", file, line, format);
    std::vprintf (buff, args);
    va_end (args);
}

void fatalPrintf(const char* format, ...)
{
    char buff[1024];
    va_list args;
    va_start (args, format);
    snprintf(buff, sizeof(buff), "FATAL ERROR: %s\n", format);
    std::vprintf(buff, args);
    va_end (args);
    exit(1);
}

void parseErrorPrintf(int line, int pos, const char* format, ...)
{
    char buff[1024];
    va_list args;
    va_start (args, format);
    snprintf(buff, sizeof(buff), "Parse error (line %d, pos %d) %s\n", line, pos, format);
    std::vprintf(buff, args);
    va_end(args);
    exit(1);

}