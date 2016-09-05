#include <regex.h>
#include <lex/dfa.h>
#include <iostream>
#include <fstream>
#include <map>
#include <lex/lex.h>

std::vector<std::string> tokens;
DFA dfa;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "usage: parse infile outfile" << std::endl;
        std::cout << "  infile:   a correctly formatted description of grammar to parse" << std::endl;
        std::cout << "  outfile:  an LALR(1) parsing table corresponding to the grammar" << std::endl;
        return 0;
    }

    /* Open the lexical specification file */
    std::ifstream spec;
    spec.open(argv[1]);

    if (!spec.is_open())
    {
        std::cerr << "error opening infile. aborting." << std::endl;
        return -1;
    }

    /* Parse each matched_lexeme into a DFA and load it into a map */
    std::string name, pattern;

    while (spec >> name)
    {
    }

    std::ofstream out(argv[2], std::ios::binary);
    out << dfa;
    out.close();
}