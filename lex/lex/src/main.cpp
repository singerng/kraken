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
    RegexParser prs;

    if (argc != 3)
    {
        std::cout << "usage: lex infile outfile" << std::endl;
        std::cout << "  infile:   a correctly formatted description of tokens to parse" << std::endl;
        std::cout << "  outfile:  a DFA corresponding to the tokens" << std::endl;
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

    RegexNode *node = NULL;

    while (spec >> name)
    {
        spec.get();
        getline(spec, pattern);
        const char *cregex = pattern.c_str();
        RegexNode *regex = prs.regex(&cregex);

        int flags = 0;

        int token_num;

        if (name.back() == ']' && name.find('d', name.find_last_of('[')) != std::string::npos) {
            token_num = LEX_DISCARD_TOKEN;
        } else {
            tokens.push_back(name);
            token_num = tokens.size()+1;
        }

        if (node) node = new UnionNode(node, new ConcatNode(regex, new Leaf(token_num, true)));
        else node = new ConcatNode(regex, new Leaf(token_num, true));
    }

    dfa = prs.parse(node);

    std::ofstream out(argv[2], std::ios::binary);
    out << dfa;
    out.close();
}