#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <iostream>
#include <variant>
#include <memory>
using namespace std;

class ExpressionError : public runtime_error {
    public:
        explicit ExpressionError(const string& message) : runtime_error(message) {}
};

class Expression {
    public:
        Expression() = default;
        virtual ~Expression() = default;

        virtual variant<int, bool> evaluate() const = 0;
};

class Literal : public Expression {
    private:
        variant<int, bool> value;
    
    public:
        explicit Literal(variant<int, bool> v) : value(v) {}

        inline variant<int, bool> evaluate() const override { return value; }
};

class PrimaryExpression : public Expression {
    private:
        unique_ptr<Expression> expression;
        bool Parenthesized;
    
    public:
        explicit PrimaryExpression(unique_ptr<Expression> expr, bool parenthesis = false) 
            : expression(move(expr)), Parenthesized(parenthesis) {
            if (!expression) {
                throw ExpressionError("Não é possível criar PrimaryExpression a partir de uma expressão nula");
            }
        }

        inline variant<int, bool> evaluate() const override { 
            return expression->evaluate(); 
        }

        inline bool isParenthesized() const { return Parenthesized; }
};

class UnaryExpression : public Expression {
    private:
        unique_ptr<Expression> expression;
        string operador;

    public:
        explicit UnaryExpression(string operador, unique_ptr<Expression> expr) 
            : operador(operador), expression(move(expr)) {
            if (!expression) {
                throw ExpressionError("Não é possível criar uma UnaryExpression a partir de uma expressão nula");
            }
        }

        variant<int, bool> evaluate() const override {
            auto value = expression->evaluate();

            if (holds_alternative<int>(value)) {
                if (operador == "-") {
                    return -get<int>(value);
                }
                throw ExpressionError("Operador Unário para Inteiros inválido: " + operador);
            } 
            else if (holds_alternative<bool>(value)) {
                throw ExpressionError("Operador Unário para Booleanos inválido: " + operador);
            }
            return value;
        }
};

class BinaryExpression : public Expression {
    private:
        string operador;
        unique_ptr<Expression> left;
        unique_ptr<Expression> right;
    
    public:
        // Construtor para Expressions
        explicit BinaryExpression(unique_ptr<Expression> left, string operador, unique_ptr<Expression> right) 
            : left(move(left)), operador(operador), right(move(right)) {
            if (!this->left || !this->right){
                throw ExpressionError("Não é possível criar uma BinaryExpression com operandos nulos");
            }
        }

        variant<int, bool> evaluate() const override {
            auto left_value = left->evaluate();
            auto right_value = right->evaluate();
            
            if (holds_alternative<int>(left_value) && holds_alternative<int>(right_value)) {
                int lv = get<int>(left_value);
                int rv = get<int>(right_value);

                if (operador == "+") return lv + rv;
                if (operador == "-") return lv - rv;
                if (operador == "*") return lv * rv;
                if (operador == "/") {
                    if (rv == 0) throw ExpressionError("Divisão por zero");
                    return lv / rv;
                }
                if (operador == "<") return lv < rv;
                if (operador == ">") return lv > rv;
                if (operador == "<=") return lv <= rv;
                if (operador == ">=") return lv >= rv;
                if (operador == "==") return lv == rv;
                if (operador == "!=") return lv != rv;
                throw ExpressionError("Avaliando um operador aritmético binário desconhecido");
            } 
            else if (holds_alternative<bool>(left_value) && holds_alternative<bool>(right_value)) {
                bool lv = get<bool>(left_value);
                bool rv = get<bool>(right_value);

                if (operador == "&&") return lv && rv;
                if (operador == "||") return lv || rv;
                if (operador == "==") return lv == rv;
                if (operador == "!=") return lv != rv;
                throw ExpressionError("Avaliando um operador lógico binário desconhecido");
            }
            throw ExpressionError("Avaliando operandos de tipos diferentes");
        }
};

#endif