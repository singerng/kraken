#ifndef __LEX_H
#define __LEX_H

#include <iostream>

#define END_LEX		-1

class Lexer {
public:
    Lexer(DFA dfa);

    /* Initializes the lexer to a given input stream (usually a file) */
    int init(std::istream *stream);

    /* Gets the next token, or END_LEX if none exists */
    int next_token();

    /* Gets the current line number */
    int line_number();

private:
    DFA dfa;
    char buffer[(BLOCK_SIZE+1)*2];
    char *forward = buffer;
    char *back = buffer;
    std::istream *in;

    void fill_buffer(int half);
    char cur_char();
    char next_char();
    char back(int back);
};

#endif