cmake_minimum_required(VERSION 3.6)
project(Chile)

set(CMAKE_CXX_STANDARD 14)

add_library(logger utils/Logger.cpp utils/Logger.h)

set(SOURCE_FILES main.cpp Foodie.h Foodie.cpp Tokenizer.h Tokenizer.cpp Token.h Token.cpp Parser.cpp Parser.h Expression.cpp Expression.h)
add_executable(Foodie ${SOURCE_FILES})

target_link_libraries(Foodie logger)

target_compile_definitions(Foodie
        PRIVATE DEBUG=1
        )