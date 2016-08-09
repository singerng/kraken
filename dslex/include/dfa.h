#ifndef __DFA_H
#define __DFA_H

#include <vector>
#include <array>

#define DFA_ERROR   -1
#define DFA_OK      0
#define DFA_ACCEPT  1

struct state
{
    std::array<int, 256> trans;
    bool accept;
};

class DFA
{
public:
    DFA();

    /* Add a state to the DFA, returning its state number */
    int add_state();

    /* Mark a DFA state as accept or non-accept */
    void set_accept(int state, bool accept);

    /* Set a transition from one state of the DFA to another */
    void set_trans(int source, char value, int target);

    /* Move to a new state based on an input symbol */
    int move(char value);

    /* Get the current status of the DFA */
    int status();

    /* Reset to the start state */
    void reset();

    /* Print the DFA for debugging */
    void print();

private:
    std::vector<struct state> dfa;
    int state;
};

#endif
