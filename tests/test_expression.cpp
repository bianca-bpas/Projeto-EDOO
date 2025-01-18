#include <iostream>
#include <string>
#include "expressions.h"

using namespace std;

// Função auxiliar para imprimir resultados de testes
void printTestResult(const string& testName, bool passed) {
    cout << (passed ? "[PASS]" : "[FAIL]") << " " << testName << endl;
}

// Teste para Literais
void testLiteral() {
    Literal literalInt(42);
    printTestResult("Literal Integer", std::get<int>(literalInt.evaluate()) == 42);

    Literal literalBool(true);
    printTestResult("Literal Boolean", std::get<bool>(literalBool.evaluate()) == true);
}

// Teste para Expressões Primárias
void testPrimaryExpression() {
    Literal* literal = new Literal(99);
    PrimaryExpression expr(literal);
    printTestResult("Primary Expression Integer", std::get<int>(expr.evaluate()) == 99);

    Literal* literalBool = new Literal(false);
    PrimaryExpression exprBool(literalBool, true);
    printTestResult("Primary Expression Boolean (Parenthesized)", std::get<bool>(exprBool.evaluate()) == false);

    Literal* literaInt = new Literal(10);
    PrimaryExpression exprInt(literaInt, true);
    printTestResult("Primary Expression Integer (Parenthesized)", std::get<int>(exprInt.evaluate()) == 10);
}

// Teste para Expressões Unárias
void testUnaryExpression() {
    Literal* literal = new Literal(5);
    PrimaryExpression* primary = new PrimaryExpression(literal);
    UnaryExpression negateExpr(primary, "-");
    printTestResult("Unary Expression Negate Integer", std::get<int>(negateExpr.evaluate()) == -5);

    Literal* literalNoOp = new Literal(10);
    PrimaryExpression* primaryNoOp = new PrimaryExpression(literalNoOp);
    UnaryExpression noOpExpr(primaryNoOp, "");
    printTestResult("Unary Expression No Operation", std::get<int>(noOpExpr.evaluate()) == 10);

    Literal* literalNeg = new Literal(-10);
    PrimaryExpression* primaryNeg = new PrimaryExpression(literalNeg);
    UnaryExpression negExpr(primaryNeg, "-");
    printTestResult("Unary Expression Negate", std::get<int>(negExpr.evaluate()) == 10);
}

// Teste para Expressões Binárias
void testBinaryExpression() {
    Literal* left = new Literal(3);
    Literal* right = new Literal(4);
    BinaryExpression addExpr("+", left, right);
    printTestResult("Binary Expression Addition", std::get<int>(addExpr.evaluate()) == 7);

    Literal* leftBool = new Literal(true);
    Literal* rightBool = new Literal(false);
    BinaryExpression andExpr("&&", leftBool, rightBool);
    printTestResult("Binary Expression AND", std::get<bool>(andExpr.evaluate()) == false);

    Literal* leftDiv = new Literal(10);
    Literal* rightZero = new Literal(0);
    BinaryExpression divExpr("/", leftDiv, rightZero);
    try {
        divExpr.evaluate();
        printTestResult("Binary Expression Division by Zero", false); // Deve lançar exceção
    } catch (const ExpressionError&) {
        printTestResult("Binary Expression Division by Zero", true); // Exceção esperada
    }

    Literal* leftUnknown = new Literal(10);
    Literal* rightUnknown = new Literal(20);
    BinaryExpression unknownExpr("%", leftUnknown, rightUnknown);
    try {
        unknownExpr.evaluate();
        printTestResult("Binary Expression Unknown Operator", false); // Deve lançar exceção
    } catch (const ExpressionError&) {
        printTestResult("Binary Expression Unknown Operator", true); // Exceção esperada
    }
}

// Teste para Expressões Complexas
void testComplexExpression() {
    Literal* leftLiteral = new Literal(2);
    Literal* rightLiteral = new Literal(3);
    BinaryExpression* innerExpr = new BinaryExpression("*", leftLiteral, rightLiteral);

    Literal* outerLiteral = new Literal(5);
    BinaryExpression expr("+", innerExpr, outerLiteral);

    printTestResult("Complex Expression Nested Binary", std::get<int>(expr.evaluate()) == 11);
}

// Função principal para executar os testes
int main() {
    cout << "=== Testes de Expressions ===" << endl;

    testLiteral();
    testPrimaryExpression();
    testUnaryExpression();
    testBinaryExpression();
    testComplexExpression();

    return 0;
}
