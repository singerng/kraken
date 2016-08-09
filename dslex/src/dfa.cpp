#include <dfa.h>
#include <iostream>
#include <array>
#include <algorithm>

DFA::DFA() {
}

int DFA::add_state() {
    std::array<int, 256> trans;
    std::fill(trans.begin(), trans.end(), -1);

    this->dfa.push_back(trans);

    return dfa.size()-1;
}

void DFA::set_trans(int source, char value, int dest) {
    if (dest < 0 || dest >= dfa.size()) printf("error: setting dest to value %d\n", dest);
    dfa[source][value] = dest;
}

void DFA::print() {
    for (int state = 0; state < dfa.size(); state++) {
        std::cout << "[state " << state << "]" << std::endl;

        for (int c = 0; c < 256; c++) {
            if (dfa[state][c] != -1) {
                std::cout << c << " '" << (unsigned char) c << "' -> " << dfa[state][c] << std::endl;
            }
        }

        std::cout << std::endl;
    }
}