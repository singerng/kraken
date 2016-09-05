#include <lex/dfa.h>
#include <lex/lex.h>
#include <iostream>
#include <fstream>
#include <map>

char Lexer::cur_char()
{
    return *forward;
}

char Lexer::next_char()
{
    return *(forward++);
}

void Lexer::retreat(int back)
{
    forward -= back;
}

std::string Lexer::matched_lexeme() {
    return std::string(back, forward-back);
}

int Lexer::next_token(struct token &token)
{
    int last_match = DFA_OK;
    int match_pos = 0;

    if (cur_char() == NULL) return END_LEX;

    while (dfa.move(next_char()) != DFA_ERROR)
    {
        if (dfa.status() > DFA_OK)
        {
            last_match = dfa.status();
            match_pos = dfa.move_count();
        }

        if (cur_char() == '\n')
        {
            line++;
            pos = 0;
        }
        else pos++;
    }

    if (dfa.move_count() == 1 && dfa.status() == DFA_ERROR)
    {
        std::cerr << "error: unexpected character '" << cur_char() << "' at line " << line << ", position " << pos << std::endl;
        return END_LEX;
    }

    retreat(dfa.move_count() - match_pos);
    dfa.reset();

    token.lexeme = this->matched_lexeme();
    back = forward;

    if (last_match == LEX_DISCARD_TOKEN) return next_token(token);

    token.token = last_match-2;

    return CONTINUE_LEX;
}

void Lexer::init(std::istream *in, int size)
{
    buffer = new char[size+1];
    forward = buffer;
    back = buffer;

    in->read(buffer, size);
    buffer[size] = '\0';
}

Lexer::Lexer(DFA dfa)
{
    this->dfa = dfa;
}

int Lexer::line_number()
{
    return line;
}

int Lexer::position()
{
    return pos;
}