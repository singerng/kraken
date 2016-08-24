#include <fstream>
#include <lex/dfa.h>
#include <lex/lex.h>

int main(int argc, char **argv)
{
    DFA dfa;

    std::ifstream dfa_file("kraken.dfa", std::ios::binary);

    if (!dfa_file.is_open())
    {
        std::cerr << "error opening dfa. aborting." << std::endl;
        return -1;
    }

    dfa_file >> dfa;

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

    /* Create the lexer */
    Lexer lex(dfa);
    lex.init(&code, size);

    struct token tok;
    while (lex.next_token(tok) == CONTINUE_LEX) std::cout << tok.id << "," << tok.token << std::endl;

    return 0;
}