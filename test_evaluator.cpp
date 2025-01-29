#include "parser.h"
#include <cassert>
#include <iostream>

void test_arithmetic() {
    ExpressionEvaluator eval;
    
    assert(get<int>(eval.evaluate("2 + 3")) == 5);
    assert(get<int>(eval.evaluate("10 - 4")) == 6);
    assert(get<int>(eval.evaluate("3 * 4")) == 12);
    assert(get<int>(eval.evaluate("8 / 2")) == 4);
    cout << "Testes aritméticos passaram!\n";
}

void test_relational() {
    ExpressionEvaluator eval;
    
    assert(get<bool>(eval.evaluate("5 < 10")) == true);
    assert(get<bool>(eval.evaluate("15 > 5")) == true);
    assert(get<bool>(eval.evaluate("8 <= 8")) == true);
    assert(get<bool>(eval.evaluate("9 >= 10")) == false);
    cout << "Testes relacionais passaram!\n";
}

void test_logical() {
    ExpressionEvaluator eval;
    
    assert(get<bool>(eval.evaluate("true && true")) == true);
    assert(get<bool>(eval.evaluate("false || true")) == true);
    assert(get<bool>(eval.evaluate("5 == 5")) == true);
    assert(get<bool>(eval.evaluate("6 != 7")) == true);
    cout << "Testes lógicos passaram!\n";
}

void test_parentheses() {
    ExpressionEvaluator eval;
    
    assert(get<int>(eval.evaluate("(2 + 3) * 4")) == 20);
    assert(get<int>(eval.evaluate("2 + (3 * 4)")) == 14);
    assert(get<bool>(eval.evaluate("(5 > 3) && (2 < 4)")) == true);
    cout << "Testes com parênteses passaram!\n";
}

void test_errors() {
    ExpressionEvaluator eval;
    bool caught_error = false;
    
    try {
        eval.evaluate("10 / 0");
    } catch (const exception&) {
        caught_error = true;
    }
    assert(caught_error);
    
    caught_error = false;
    try {
        eval.evaluate("2 + true");
    } catch (const exception&) {
        caught_error = true;
    }
    assert(caught_error);
    
    cout << "Testes de erro passaram!\n";
}

int main() {
    cout << "Iniciando testes do avaliador de expressões...\n";
    
    test_arithmetic();
    test_relational();
    test_logical();
    test_parentheses();
    test_errors();
    
    cout << "\nTodos os testes passaram com sucesso!\n";
    return 0;
}
