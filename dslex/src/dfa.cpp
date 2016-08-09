#include <dfa.h>
#include <iostream>
#include <array>
#include <algorithm>

DFA::DFA() {
}

int DFA::add_state() {
    /* Allocate a new state with default properties (no transitions out, non-accept )*/
    struct state state;
    std::fill(state.trans.begin(), state.trans.end(), DFA_ERROR);
    state.accept = false;

    /* Add the state to the DFA */
    dfa.push_back(state);

    /* Return the state number of the state we just added */
    return dfa.size()-1;
}

void DFA::set_accept(int state, bool accept) {
    /* Mark the given state as an accept or non-accept state */
    dfa[state].accept = accept;
}

void DFA::set_trans(int source, char value, int dest) {
    /* Check that this is a valid destination state */
    if (dest < 0 || dest >= dfa.size()) printf("error: setting dest to value %d\n", dest);

    /* Mark the transition to the destination state */
    dfa[source].trans[value] = dest;
}

int DFA::move(char value) {
    state = dfa[state].trans[value];

    return status();
}

int DFA::status() {
    if (state == DFA_ERROR) return DFA_ERROR;
    else if (dfa[state].accept) return DFA_ACCEPT;
    else return DFA_OK;
}

void DFA::reset() {
    state = 0;
}

void DFA::print() {
    for (int state = 0; state < dfa.size(); state++) {
        std::cout << "[state " << state << "]";
        if (dfa[state].accept) std::cout << " [ACCEPT]";
        std::cout << std::endl;

        for (int c = 0; c < 256; c++) {
            if (dfa[state].trans[c] != -1) {
                std::cout << c << " '" << (unsigned char) c << "' -> " << dfa[state].trans[c] << std::endl;
            }
        }

        std::cout << std::endl;
    }
}