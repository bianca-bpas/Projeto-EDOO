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
        const string text;
        size_t pos;
        char current_char;

        // Funções auxiliares
        void error(const string& message) const {
            throw LexerError(message);
        }

        void advance() {
            /* Avança para o próximo caractere */
            if (pos < text.length()){
                pos++;
                current_char = (pos < text.length()) ? text[pos] : '\0';
            }
        }

        void skip_whitespace() {
            /* Pula espaço em branco */
            while (current_char != '\0' && isspace(current_char)) {
                advance();
            }
        }

        int integer(string result = "") {
            /* Extrai o número inteiro a partir do current_char */
            while (current_char != '\0' && isdigit(current_char)){
               result += current_char;
               advance(); 
            }
            return stoi(result);
        }

        bool boolean() {
            /* Extrai o booleano a partir do current_char */
            string result;
            while (current_char != '\0' && isalpha(current_char)){
                result += current_char;
                advance();
            }
            return (result == "true") ? true : false;
        }

        bool is_end() const {
            /* Verifica se todo o input foi lido */
            return current_char == '\0';
        }

        bool next_char(char expected) const {
            /* Verifica o próximo caractere */
            if (pos + 1 >= text.length()) {
                return false;
            }
            return text[pos + 1] == expected;
        }

    public:
        explicit Lexer(string input) : text(input), pos(0) {
            current_char = (text.empty()) ? '\0' : text[0];
            if (text.empty()){
                error("Inut vazio");
            }
        }

        Token get_next_token() {
            /* Extrai o próximo token do input */
            while (!is_end()) {
                // Pula espaços em branco
                if (isspace(current_char)){
                    skip_whitespace();
                    continue;
                }

                // Números inteiros
                if (isdigit(current_char)) {
                    return Token("INTEGER", integer());
                }

                // Booleanos
                if (isalpha(current_char)) {
                    return Token("BOOLEAN", boolean());
                }

                // Operadores Aritméticos
                if (current_char == '+') {
                    advance();
                    return Token("PLUS", "+");
                }
                if (current_char == '-') {
                    advance();
                    if (isspace(current_char)){
                        return Token("MINUS", "-");
                    }
                    return Token("INTEGER", integer("-"));
                }
                if (current_char == '*') {
                    advance();
                    return Token("MULTIPLY", "*");
                }
                if (current_char == '/') {
                    advance();
                    return Token("DIVIDE", "/");
                }

                // Operadores Lógicos
                if (current_char == '|' && next_char('|')) {
                    advance();
                    advance();
                    return Token("OR", "||");
                }
                if (current_char == '&' && next_char('&')) {
                    advance();
                    advance();
                    return Token("AND", "&&");
                }
                if (current_char == '=' && next_char('=')) {
                    advance();
                    advance();
                    return Token("EQUALS", "==");
                }
                if (current_char == '!' && next_char('=')) {
                    advance();
                    advance();
                    return Token("NOT_EQUALS", "!=");
                }
                if (current_char == '<') {
                    advance();
                    if (current_char == '=') {
                        advance();
                        return Token("LESS_EQUAL", "<=");
                    }
                    return Token("LESS", "<");
                }
                if (current_char == '>') {
                    advance();
                    if (current_char == '=') {
                        advance();
                        return Token("GREATER_EQUAL", ">=");
                    }
                    return Token("GREATER", ">");
                }

                // Parênteses
                if (current_char == '(') {
                    advance();
                    return Token("LPAREN", "(");
                }
                if (current_char == ')') {
                    advance();
                    return Token("RPAREN", ")");
                }
                error("Token desconhecido");
            }
            return Token("EOF", "");
        }
};

#endif