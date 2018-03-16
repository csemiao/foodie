#include <iostream>
#include "Foodie.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        // one arg
        // TODO: support a shell
    }
    else if (argc == 2)
    {
        unique_ptr<Foodie> p_foodie = make_unique<Foodie>();
        p_foodie->run(argv[1]);
    }
    else
    {
        std::cout << "Usage: Foodie <file>" << std::endl;
    }
}