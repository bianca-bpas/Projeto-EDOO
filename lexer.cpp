#include "lexer.h"

void Lexer::error(const string& message) {
    throw LexerError(message);
}

void Lexer::advance() {
    if (pos < text.length()){
        pos++;
        current_char = (pos < text.length()) ? text[pos] : '\0';
    }
}

void Lexer::skip_whitespace() {
    while (current_char != '\0' && isspace(current_char)) {
        advance();
    }
}

int Lexer::integer(string result) {
    while (current_char != '\0' && isdigit(current_char)){
       result += current_char;
       advance(); 
    }
    return stoi(result);
}

bool Lexer::boolean() {
    string result;
    while (current_char != '\0' && isalpha(current_char)){
        result += current_char;
        advance();
    }
    return (result == "true") ? true : false;
}

Token Lexer::get_next_token() {
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
