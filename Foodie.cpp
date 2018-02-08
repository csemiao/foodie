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

using namespace std;

class FileHandle
{
public:
    // A RAII inspired class
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

    Tokenizer tokenizer;
    shared_ptr<string> p_contents = std::make_shared<string>(contents);
    vector<Token> tokens = tokenizer.tokenize(p_contents);

    Parser parser;
    parser.parse(tokens);
}
