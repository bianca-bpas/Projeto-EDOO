#include <iostream>
#include <vector>
#include "token.h"
#include "lexer.h"
using namespace std;

// Função auxiliar para imprimir tokens de forma formatada
void print_tokens(const vector<Token>& tokens) {
    cout << "[\n";
    for (const auto& token : tokens) {
        cout << "  " << token.to_string();
        if (&token != &tokens.back()) {
            cout << ",";
        }
        cout << "\n";
    }
    cout << "]\n";
}

// Função para testar uma expressão específica
void test_expression(const string& expression) {
    cout << "\nTestando expressao: \"" << expression << "\"\n";
    cout << "----------------------------------------\n";
    
    try {
        // Criar o lexer com a expressão
        Lexer lexer(expression);
        
        // Coletar todos os tokens em um vetor
        vector<Token> tokens;
        while (true) {
            Token token = lexer.get_next_token();
            if (token.get_type() == "EOF") break;
            tokens.push_back(token);
        }
        
        // Imprimir os tokens coletados
        print_tokens(tokens);
        
    } catch (const runtime_error& e) {
        cerr << "Erro durante a análise léxica: " << e.what() << "\n";
    }
}

int main() {
    test_expression("7");
    test_expression("1");
    test_expression("2 + 3 * 2");
    test_expression("( 2 - - -3 ) * 2");
    test_expression("3 / 2");
    test_expression("true || false == false");
    test_expression("( true || false ) == false");
    test_expression("true + 3");
    
    return 0;
}
