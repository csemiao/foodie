//
// Created by Chris on 2018-01-31.
//


#include <fstream>
#include <sstream>
#include <vector>

#include "Foodie.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Interpreter.h"

using namespace std;

class FileHandle
{
public:
    FileHandle(char* filename)
    {
        file.open(filename);

        if (!file)
        {
            fatalPrintf("Could not open file: %s", filename);
        }
    }

    ~FileHandle() {
        file.close();
    }

    string getContents()
    {
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

private:
    ifstream file;

};

int Foodie::run(char* filename)
{
    FileHandle fileHandle(filename);
    string contents = fileHandle.getContents();

    shared_ptr<string> p_contents = std::make_shared<string>(contents);

    Tokenizer tokenizer;
    vector<Token> tokens = tokenizer.tokenize(p_contents);

    Parser parser;
    vector<shared_ptr<Statement>> statements = parser.parse(tokens);

    Interpreter interpreter;
    interpreter.interpret(statements);
}
