#include <dfa.h>
#include <lex.h>
#include <iostream>
#include <fstream>
#include <map>

#define BLOCK_SIZE  1024

void Lexer::fill_buffer(int half) {
    in->get(buffer + (BLOCK_SIZE+1)*half*sizeof(char), BLOCK_SIZE+1, NULL);
    buffer[(BLOCK_SIZE+1)*(half+1)-1] = NULL;
}

char Lexer::cur_char() {
    return *forward;
}

char Lexer::next_char() {
    forward++;
    if (*forward == NULL) {
        if (forward == buffer + BLOCK_SIZE) {
            fill_buffer(1);
            forward++;
        } else if (forward == buffer + BLOCK_SIZE*2 + 1) {
            fill_buffer(0);
            forward = buffer;
        } else return NULL;
    }
    return *forward;
}

char Lexer::back(int back) {
    /* TODO: seek back when it's not contained within the buffer */
    if (forward >= buffer + BLOCK_SIZE + 1 && forward - back < buffer + BLOCK_SIZE + 1) {
        forward -= back - 1;
    } else if (forward >= buffer && forward - back < buffer) {
        forward += 2*(BLOCK_SIZE+1) - back - 1;
    } else forward -= back;
}

int Lexer::next_token() {
    int last_match = DFA_OK;
    int match_pos = 0;

    if (cur_char() == NULL) return END_LEX;

    while (dfa.move(next_char()) != DFA_ERROR) {
        if (dfa.status() > DFA_OK) {
            last_match = dfa.status();
            match_pos = dfa.move_count();
        }
    }

    back(dfa.move_count() - match_pos);
    dfa.reset();

    return last_match-1;
}
