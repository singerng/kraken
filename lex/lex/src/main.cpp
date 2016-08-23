#include <regex.h>
#include <dfa.h>
#include <iostream>
#include <fstream>
#include <map>

std::vector<std::string> tokens;
DFA dfa;

int main(int argc, char **argv) {
    RegexParser prs;

    if (argc != 3) {
        std::cout << "usage: lex infile outfile" << std::endl;
        std::cout << "  infile:   a correctly formatted description of tokens to parse" << std::endl;
        std::cout << "  outfile:  a DFA corresponding to the tokens" << std::endl;
        return 0;
    }

    /* Open the lexical specification file */
    std::ifstream spec;
    spec.open(argv[1]);

    if (!spec.is_open()) {
        std::cerr << "error opening infile. aborting." << std::endl;
        return 0;
    }

    /* Parse each token into a DFA and load it into a map */
    std::string name, pattern;

    RegexNode *node = NULL;

    while (spec >> name) {
        spec.get();
        getline(spec, pattern);
        const char *cregex = pattern.c_str();
        RegexNode *regex = prs.regex(&cregex);

        tokens.push_back(name);

        if (node) node = new UnionNode(node, new ConcatNode(regex, new Leaf(tokens.size(), true)));
        else node = new ConcatNode(regex, new Leaf(tokens.size(), true));
    }

    dfa = prs.parse(node);

    std::ofstream out;
    out.open(argv[2]);
}