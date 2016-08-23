#include <lex/dfa.h>
#include <iostream>
#include <algorithm>
#include <endian.h>

DFA::DFA()
{
    reset();
}

int DFA::add_state()
{
    /* Allocate a new state with default properties (no transitions out, non-accept )*/
    struct state state;
    std::fill(state.trans.begin(), state.trans.end(), DFA_ERROR);
    state.accept = DFA_OK;

    /* Add the state to the DFA */
    dfa.push_back(state);

    /* Return the state number of the state we just added */
    return dfa.size()-1;
}

int DFA::num_states()
{
    return dfa.size();
}

void DFA::set_accept(int state, int accept)
{
    /* Mark the given state as an accept or non-accept state */
    dfa[state].accept = accept;
}

void DFA::set_trans(int source, char value, int dest)
{
    /* Check that this is a valid destination state */
    if (dest < DFA_ERROR || dest >= (int) dfa.size()) std::cerr << "error: setting dest to value " << dest << std::endl;

    /* Mark the transition to the destination state */
    dfa[source].trans[value] = dest;
}

int DFA::move(char value)
{
    state = dfa[state].trans[value];
    count++;

    return status();
}

int DFA::move_count()
{
    return count;
}

int DFA::status()
{
    if (state == DFA_ERROR) return DFA_ERROR;
    else return dfa[state].accept;
}

void DFA::reset()
{
    state = 0;
    count = 0;
}

void DFA::print()
{
    for (int state = 0; state < dfa.size(); state++)
    {
        std::cout << "[state " << state << "]";
        if (dfa[state].accept > DFA_OK) std::cout << " [ACCEPT " << dfa[state].accept << "]";
        std::cout << std::endl;

        for (int c = 0; c < 256; c++)
        {
            if (dfa[state].trans[c] != -1) std::cout << c << " '" << (unsigned char) c << "' -> " << dfa[state].trans[c] << std::endl;
        }

        std::cout << std::endl;
    }
}

std::ostream& operator<<(std::ostream &out, DFA &dfa)
{
    int i;
    i = htole32(dfa.dfa.size());
    out.write((const char*) &i, sizeof(int));

    for (int state = 0; state < dfa.num_states(); state++)
    {
        i = htole32(dfa.dfa[state].accept);
        out.write((const char*) &i, sizeof(int));
        for (int c = 0; c < 256; c++)
        {
            i = htole32(dfa.dfa[state].trans[c]);
            out.write((const char*) &i, sizeof(int));
        }
    }

    return out;
}

std::istream& operator>>(std::istream &in, DFA &dfa) {
    int states;
    in.read((char*) &states, sizeof(int));
    states = le32toh(states);

    for (int state = 0; state < states; state++) dfa.add_state();

    for (int state = 0; state < states; state++)
    {
        int i;
        in.read((char*) &i, sizeof(int));
        dfa.set_accept(state, le32toh(i));

        for (int c = 0; c < 256; c++)
        {
            in.read((char*) &i, sizeof(int));
            dfa.set_trans(state, c, le32toh(i));
        }
    }

    return in;
}