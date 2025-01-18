#include <iostream>
#include <cassert>
#include <stdexcept>
#include <variant>
#include "parser.h"
#include "lexer.h"
#include "expressions.h"

// Helper function to compare variant values
bool compareVariant(const std::variant<int, bool>& result, const std::variant<int, bool>& expected) {
    return result == expected;
}

// Função para testar expressões aritméticas
void test_arithmetic_expressions() {
    std::cout << "Testando expressões aritméticas..." << std::endl;
    
    ExpressionEvaluator evaluator;

    struct TestCase {
        std::string input;
        std::variant<int, bool> expected;
    };

    TestCase cases[] = {
        {"1 + 2", 3},
        {"10 + 20", 30},
        {"5 - 3", 2},
        {"20 - 10", 10},
        {"4 * 5", 20},
        {"7 * 6", 42},
        {"15 / 3", 5},
        {"100 / 10", 10}
    };

    for (const auto& test : cases) {
        auto result = evaluator.evaluate(test.input);
        assert(compareVariant(result, test.expected));
        std::cout << "Teste: " << test.input << " OK" << std::endl;
    }

    std::cout << "Testes de expressões aritméticas concluídos com sucesso!" << std::endl;
}

// Função para testar expressões booleanas
void test_boolean_expressions() {
    std::cout << "Testando expressões booleanas..." << std::endl;
    
    ExpressionEvaluator evaluator;

    struct TestCase {
        std::string input;
        std::variant<int, bool> expected;
    };

    TestCase cases[] = {
        {"true && true", true},
        {"true && false", false},
        {"false || true", true},
        {"false || false", false},
        {"5 < 10", true},
        {"10 > 5", true},
        {"5 <= 5", true},
        {"10 >= 10", true},
        {"5 == 5", true},
        {"5 != 10", true}
    };

    for (const auto& test : cases) {
        auto result = evaluator.evaluate(test.input);
        assert(compareVariant(result, test.expected));
        std::cout << "Teste: " << test.input << " OK" << std::endl;
    }

    std::cout << "Testes de expressões booleanas concluídos com sucesso!" << std::endl;
}

// Função para testar expressões complexas
void test_complex_expressions() {
    std::cout << "Testando expressões complexas..." << std::endl;
    
    ExpressionEvaluator evaluator;

    struct TestCase {
        std::string input;
        std::variant<int, bool> expected;
    };

    TestCase cases[] = {
        {"(1 + 2) * 3", 9},
        {"10 / (2 + 3)", 2},
        {"(5 < 10) && (3 > 1)", true},
        {"(5 > 10) || (3 < 1)", false}
    };

    for (const auto& test : cases) {
        auto result = evaluator.evaluate(test.input);
        assert(compareVariant(result, test.expected));
        std::cout << "Teste: " << test.input << " OK" << std::endl;
    }

    std::cout << "Testes de expressões complexas concluídos com sucesso!" << std::endl;
}

// Função para testar tratamento de erros
void test_error_handling() {
    std::cout << "Testando tratamento de erros..." << std::endl;
    
    ExpressionEvaluator evaluator;

    // Teste de divisão por zero
    try {
        evaluator.evaluate("10 / 0");
        std::cerr << "Erro: Divisão por zero não detectada!" << std::endl;
        assert(false);
    } catch (const ExpressionError& e) {
        std::cout << "Divisão por zero corretamente tratada: " << e.what() << std::endl;
    }

    // Teste de expressão inválida
    try {
        evaluator.evaluate("5 ++ 3");
        std::cerr << "Erro: Expressão inválida não detectada!" << std::endl;
        assert(false);
    } catch (const std::runtime_error& e) {
        std::cout << "Expressão inválida corretamente tratada: " << e.what() << std::endl;
    }

    std::cout << "Testes de tratamento de erros concluídos com sucesso!" << std::endl;
}

int main() {
    try {
        test_arithmetic_expressions();
        test_boolean_expressions();
        test_complex_expressions();
        test_error_handling();

        std::cout << "Todos os testes foram concluídos com sucesso!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Erro durante os testes: " << e.what() << std::endl;
        return 1;
    }
}