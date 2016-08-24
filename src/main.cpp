#include <fstream>
#include <lex/dfa.h>
#include <lex/lex.h>

int main(int argc, char **argv)
{
    DFA dfa;

    std::ifstream dfa_file("kraken.dfa", std::ios::binary);
    dfa_file.open("kraken.dfa");

    if (!dfa_file.is_open())
    {
        std::cerr << "error opening dfa. aborting." << std::endl;
        return -1;
    }

    dfa_file >> dfa;

    dfa.print();

    if (argc != 2)
    {
        std::cout << "usage: kraken infile" << std::endl;
        std::cout << "  infile:   a Kraken program" << std::endl;
        return -1;
    }

    /* Open the code we're compiling */
    std::ifstream code(argv[1], std::ios::ate | std::ios::binary);

    if (!code.is_open())
    {
        std::cerr << "error opening code file. aborting." << std::endl;
        return -1;
    }

    int size = code.tellg();
    code.seekg(0);

    std::cout << size << std::endl;

    /* Create the lexer */
    Lexer lex(dfa);
    lex.init(&code, size);

    return 0;
}