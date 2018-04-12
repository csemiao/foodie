cmake_minimum_required(VERSION 3.10)
project(Chile_2)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    utils/Logger.cpp
    utils/Logger.h
    Expression.cpp
    Expression.h
    Foodie.cpp
    Foodie.h
    Interpreter.cpp
    Interpreter.h
    main.cpp
    Parser.cpp
    Parser.h
    Statement.cpp
    Statement.h
    Token.cpp
    Token.h
    Tokenizer.cpp
    Tokenizer.h utils/common.h)

add_executable(Chile_2 ${SOURCE_FILES})