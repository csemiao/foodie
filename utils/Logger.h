//
// Created by Chris on 2018-01-31.
//

#pragma once

#ifdef DEBUG
#define dbPrintf(format, ...) dbPrintfImpl(__FILE__, __LINE__, format, __VA_ARGS__)
#else
#define dbPrintf(format, ...)
#endif

// print a debug message with file and line prepended
void dbPrintfImpl(const char* file, int line, const char* format, ...);

// Print an error message and exit the program.
void fatalPrintf(const char* format, ...);

void parseErrorPrintf(int line, int pos, const char* format, ...);
