#include <regex.h>
#include <dfa.h>
#include <iostream>
#include <fstream>
#include <map>

#define BLOCK_SIZE  1024

std::vector<std::string> tokens;
DFA dfa;
char buffer[(BLOCK_SIZE+1)*2];
char *forward = buffer;

void fill_buffer(std::istream *in, int half) {
    in->get(buffer + (BLOCK_SIZE+1)*half*sizeof(char), BLOCK_SIZE+1, NULL);
    buffer[(BLOCK_SIZE+1)*(half+1)-1] = NULL;
}

char cur_char() {
    return *forward;
}

char next_char(std::istream *in) {
    forward++;
    if (*forward == NULL) {
        if (forward == buffer + BLOCK_SIZE) {
            fill_buffer(in, 1);
            forward++;
        } else if (forward == buffer + BLOCK_SIZE*2 + 1) {
            fill_buffer(in, 0);
            forward = buffer;
        } else return NULL;
    }
    return *forward;
}

char back(int back) {
    if (forward >= buffer + BLOCK_SIZE + 1 && forward - back < buffer + BLOCK_SIZE + 1) {
        forward -= back - 1;
    } else if (forward >= buffer && forward - back < buffer) {
        forward += 2*(BLOCK_SIZE+1) - back - 1;
    } else forward -= back;
}

int next_token(std::istream *in) {
    int last_match = DFA_OK;
    int match_pos = 0;

    if (cur_char() == NULL) return END_LEX;

    while (dfa.move(next_char(in)) != DFA_ERROR) {
        if (dfa.status() > DFA_OK) {
            last_match = dfa.status();
            match_pos = dfa.move_count();
        }
    }

    back(dfa.move_count() - match_pos);
    dfa.reset();

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

    std::ifstream in;
    in.open(argv[2]);

    fill_buffer(&in, 0);

    int tok;
    while ((tok = next_token(&in)) != -1) {
        std::cout << tokens[tok] << std::endl;
    }
}
