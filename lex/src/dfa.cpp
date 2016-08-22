#include <dfa.h>
#include <iostream>
#include <algorithm>

DFA::DFA() {
    reset();
}

int DFA::add_state() {
    /* Allocate a new state with default properties (no transitions out, non-accept )*/
    struct state state;
    std::fill(state.trans.begin(), state.trans.end(), DFA_ERROR);
    state.accept = DFA_OK;

    /* Add the state to the DFA */
    dfa.push_back(state);

    /* Return the state number of the state we just added */
    return dfa.size()-1;
}

int DFA::num_states() {
    return dfa.size();
}

void DFA::set_accept(int state, int accept) {
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
    count++;

    return status();
}

int DFA::move_count() {
    return count;
}

int DFA::status() {
    if (state == DFA_ERROR) return DFA_ERROR;
    else return dfa[state].accept;
}

void DFA::reset() {
    state = 0;
    count = 0;
}

void DFA::print() {
    for (int state = 0; state < dfa.size(); state++) {
        std::cout << "[state " << state << "]";
        if (dfa[state].accept > DFA_OK) std::cout << " [ACCEPT " << dfa[state].accept << "]";
        std::cout << std::endl;

        for (int c = 0; c < 256; c++) {
            if (dfa[state].trans[c] != -1) {
                std::cout << c << " '" << (unsigned char) c << "' -> " << dfa[state].trans[c] << std::endl;
            }
        }

        std::cout << std::endl;
    }
}