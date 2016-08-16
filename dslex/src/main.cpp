#include <regex.h>
#include <dfa.h>
#include <iostream>
#include <fstream>
#include <map>

struct token {
    std::string name;
    DFA dfa;
};

std::vector<struct token> tokens;

std::string next_token(std::istream in) {
    std::string lex;
    in >> lex;
    const char *clex = lex.c_str();
    int since = 0;

    std::vector<struct token*> viable;
    for (auto token : tokens) {
        viable.push_back(new struct token(token));
    }

    struct token *best = NULL;

    while (viable.size() >= 1) {
        for (auto iter = viable.begin(); iter < viable.end();) {
            (*iter)->dfa.move(*(clex++));

            if ((*iter)->dfa.status() == DFA_ERROR) {
                auto prev = iter++;
                viable.erase(prev);
            } else {
                if ((*iter)->dfa.status() == DFA_ACCEPT) {
                    best = *iter;
                }
                iter++;
            }
        }

        since++;
    }

    return best->name;
}

int main(int argc, char **argv) {
    RegexParser prs;

    /* Open the lexical specification file */
    std::ifstream spec;
    spec.open(argv[1]);

    /* Parse each token into a DFA and load it into a map */
    std::string name, pattern;

    while (spec >> name >> pattern) {
        const char *cregex = pattern.c_str();
        DFA dfa = prs.parse(&cregex);
        struct token token;
        token.name = name;
        token.dfa = dfa;
        tokens.push_back(token);
    }

    while (1) {
        std::string tkn = next_token(std::cin);
        std::cout << tkn << std::endl;
    }
}
