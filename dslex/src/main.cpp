#include <regex.h>
#include <dfa.h>
#include <iostream>
#include <fstream>
#include <map>

std::vector<std::string> tokens;
DFA dfa;

int next_token(std::istream &in) {
    std::string lex;
    in >> lex;
    const char *clex = lex.c_str();

    int last_match = DFA_OK;

    while (dfa.move(*(clex++)) != DFA_ERROR) {
        if (dfa.status() > DFA_OK) last_match = dfa.status();
    }

    return last_match-1;
}

int main(int argc, char **argv) {
    RegexParser prs;

    /* Open the lexical specification file */
    std::ifstream spec;
    spec.open(argv[1]);

    /* Parse each token into a DFA and load it into a map */
    std::string name, pattern;

    RegexNode *node = NULL;

    while (spec >> name >> pattern) {
        const char *cregex = pattern.c_str();
        RegexNode *regex = prs.regex(&cregex);

        tokens.push_back(name);

        if (node) node = new UnionNode(node, new ConcatNode(regex, new Leaf(tokens.size(), true)));
        else node = new ConcatNode(regex, new Leaf(tokens.size(), true));
    }

    dfa = prs.parse(node);

    dfa.print();

    while (1) {
        int tkn = next_token(std::cin);

        if (tkn != DFA_ERROR) std::cout << tokens[tkn] << std::endl;
        else std::cout << "NONE" << std::endl;
        dfa.reset();
    }
}
