#ifndef LEXER_H
#define LEXER_H

#include "token.h"
using namespace std;

class LexerError : public runtime_error {
    public:
        explicit LexerError(const string& message) : runtime_error("Erro léxico: " + message) {}
};

class Lexer {
    private:
        string text;
        size_t pos;
        char current_char;

        void error(const string& message);
        void advance();
        void skip_whitespace();
        int integer(string result = "");
        bool boolean();
        bool is_end();
        bool next_char(char expected);

    public:
        explicit Lexer(string input) : text(input), pos(0) {
            current_char = (text.empty()) ? '\0' : text[0];
            if (text.empty()){
                error("Input vazio");
            }
        }
        Token get_next_token();
};

#endif