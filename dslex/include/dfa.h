#ifndef __DFA_H
#define __DFA_H

#include <vector>
#include <array>

class DFA
{
public:
    DFA();

    /* Add a state to the DFA, returning its state number */
    int add_state();

    /* Set a transition from one state of the DFA to another */
    void set_trans(int source, char value, int target);

    /* Print the DFA for debugging */
    void print();

private:
    std::vector<std::array<int, 256>> dfa;
};

#endif
