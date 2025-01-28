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
        using ValueType = variant<int, bool>;
        Expression() = default;
        virtual ~Expression() = default;

        virtual ValueType evaluate() const = 0;
};

class Literal : public Expression {
    private:
        ValueType value;
    
    public:
        explicit Literal(ValueType v) : value(v) {}

        ValueType evaluate() const override { return value; }
};

class PrimaryExpression : public Expression {
    private:
        unique_ptr<Expression> expression;
        bool Parenthesized;
    
    public:
        explicit PrimaryExpression(unique_ptr<Expression> expr, bool parenthesis = false) 
            : expression(std::move(expr)), Parenthesized(parenthesis) {
            if (!expression) {
                throw ExpressionError("Não é possível criar PrimaryExpression a partir de uma expressão nula");
            }
        }

        variant<int, bool> evaluate() const override { 
            return expression->evaluate(); 
        }

        bool isParenthesized() const { return Parenthesized; }
};

class UnaryExpression : public Expression {
    private:
        unique_ptr<Expression> expression;
        string operador;

        static ValueType applyUnaryOperator(const string& op, const ValueType& value) {
            return visit([&op](auto&& arg) -> ValueType {
                using T = decay_t<decltype(arg)>;

                if constexpr (is_same_v<T, int>) {
                    if (op == "-") return -arg;
                    throw ExpressionError("Operador Unário para Inteiros inválido: " + op);
                } 
                throw ExpressionError("Operador Unário não suportado");
            }, value);
        }

    public:
        explicit UnaryExpression(string operador, unique_ptr<Expression> expr) 
            : operador(operador), expression(std::move(expr)) {
            if (!expression) {
                throw ExpressionError("Não é possível criar uma UnaryExpression a partir de uma expressão nula");
            }
        }

        ValueType evaluate() const override {
            auto value = expression->evaluate();

            return visit([this](auto&& arg) -> variant<int, bool> {
                using T = decay_t<decltype(arg)>;

                if constexpr (is_same_v<T, int>) {
                    if (operador == "-") {
                        return -arg;
                        throw ExpressionError("Operador Unário para Inteiros inválido: " + operador);
                    } 
                }else if constexpr (is_same_v<T, bool>) {
                    throw ExpressionError("Operador Unário para Booleanos inválido: " + operador);
                }
                return arg;
            }, value);
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
            : left(std::move(left)), operador(operador), right(std::move(right)) {
            if (!this->left || !this->right){
                throw ExpressionError("Não é possível criar uma BinaryExpression com operandos nulos");
            }
        }

        ValueType evaluate() const override {
            auto left_value = left->evaluate();
            auto right_value = right->evaluate();
            
            // Operações aritméticas e booleanas
            return visit([this](auto&& lv, auto&& rv) -> variant<int, bool> {
                using L = decay_t<decltype(lv)>;
                using R = decay_t<decltype(rv)>;

                if constexpr (is_same_v<L, int> && is_same_v<R, int>){
                    if (operador == "+"){return lv + rv;}
                    if (operador == "-"){return lv - rv;}
                    if (operador == "*"){return lv * rv;}
                    if (operador == "/"){return (rv != 0) ? lv / rv : throw ExpressionError("Divisão por zero");}
                    if (operador == "<"){return lv < rv;}
                    if (operador == ">"){return lv > rv;}
                    if (operador == "<="){return lv <= rv;}
                    if (operador == ">="){return lv >= rv;}
                    if (operador == "=="){return lv == rv;}
                    if (operador == "!="){return lv != rv;}
                    throw ExpressionError("Avaliando um operador aritmético binário desconhecido");

                } else if constexpr (is_same_v<L, bool> && is_same_v<R, bool>){
                    if (operador == "&&"){return lv && rv;}
                    if (operador == "||"){return lv || rv;}
                    if (operador == "=="){return lv == rv;}
                    if (operador == "!="){return lv != rv;}
                    throw ExpressionError("Avaliando um operador lógico binário desconhecido");

                } 
                throw ExpressionError("Avaliando operandos de tipos diferentes");
    
            }, left_value, right_value);
        }
};

#endif