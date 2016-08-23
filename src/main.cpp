#include <fstream>
#include <lex/dfa.h>

int main(int argc, char **argv)
{
    DFA dfa;

    std::ifstream dfa_file;
    dfa_file.open("kraken.dfa");

    if (!dfa_file.is_open())
    {
        std::cerr << "error opening dfa. aborting." << std::endl;
        return -1;
    }

    dfa_file >> dfa;

    dfa.print();

    return 0;
}