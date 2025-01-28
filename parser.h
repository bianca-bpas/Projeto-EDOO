#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "expressions.h"
#include <map>
#include <memory>

class ParserError : public runtime_error {
    public:
        explicit ParserError(const string& message) : runtime_error(message) {}
};

class Parser {
    private:
        Lexer lexer;
        Token current_token;

        // Funções auxiliares
        void error(const string& message) { 
            throw ParserError("Erro de sintaxe: " + message);
        }

        void advance(const string& expected_type){
            if (current_token.get_type() != expected_type){
                error("Esperado " + expected_type + ", obteve " + current_token.get_type());
            } 
            current_token = lexer.get_next_token();
        }
        

    public:
        explicit Parser(const Lexer& l) : lexer(l), current_token(lexer.get_next_token()) {}
        ~Parser() = default;

        variant<int, bool> evaluate(){
            auto expr = parse_exp();
            return expr->evaluate();
        }

        unique_ptr<Expression> parse_exp(){
            // <exp> ::= <or_exp>
            return parse_or_exp();
        }

        unique_ptr<Expression> parse_or_exp(){
            // <or_exp> ::= <and_exp> [ "||" <and_exp> ]
            auto e1 = parse_and_exp();

            if (current_token.get_type() == "OR"){
                advance("OR");
                auto e2 = parse_and_exp();
                return make_unique<BinaryExpression>(std::move(e1), "||", std::move(e2));
            }
            return e1;
        }

        unique_ptr<Expression> parse_and_exp(){
            // <and_exp> ::= <eq_exp> [ "&&" <eq_exp> ]
            auto e1 = parse_eq_exp();

            if (current_token.get_type() == "AND"){
                advance("AND");
                auto e2 = parse_eq_exp();
                return make_unique<BinaryExpression>(std::move(e1), "&&", std::move(e2));
            }
            return e1;
        }

        unique_ptr<Expression> parse_eq_exp(){
            // <eq_exp> ::= <rel_exp> [ "==" <rel_exp> | "!=" <rel_exp> ]
            auto e1 = parse_rel_exp();

            map<string, string> operadores = {
                {"EQUALS","=="},
                {"NOT_EQUALS","!="}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                auto e2 = parse_rel_exp();
                return make_unique<BinaryExpression>(std::move(e1), operador, std::move(e2));
            }
            return e1;
        }

        unique_ptr<Expression> parse_rel_exp(){
            // <rel_exp> ::= <add_exp> [ "<" <add_exp> | ">" <add_exp> | "<=" <add_exp> | ">=" <add_exp> ]
            auto e1 = parse_add_exp();

            map<string, string> operadores = {
                {"LESS","<"},
                {"GREATER",">"},
                {"LESS_EQUAL","<="},
                {"GREATER_EQUAL", ">="}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                auto e2 = parse_add_exp();
                return make_unique<BinaryExpression>(std::move(e1), operador, std::move(e2));
            }
            return e1;
        }
        
        unique_ptr<Expression> parse_add_exp(){
            // <add_exp> ::= <mul_exp> [ "+" <mul_exp> | "-" <mul_exp> ]
            auto e1 = parse_mul_exp();

            map<string, string> operadores = {
                {"PLUS","+"},
                {"MINUS","-"}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                auto e2 = parse_mul_exp();
                return make_unique<BinaryExpression>(std::move(e1), operador, std::move(e2));
            }
            return e1;
        }

        unique_ptr<Expression> parse_mul_exp(){
            // <mul_exp> ::= <unary_exp> [ "*" <unary_exp> | "/" <unary_exp> ]
            auto e1 = parse_unary_exp();

            map<string, string> operadores = {
                {"MULTIPLY","*"},
                {"DIVIDE","/"}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                auto e2 = parse_unary_exp();
                return make_unique<BinaryExpression>(std::move(e1), operador, std::move(e2));
            }
            return e1;
        }

        unique_ptr<Expression> parse_unary_exp(){
            // <unary_exp> ::= "-" <primary_exp> | <primary_exp>
            if (current_token.get_type() == "MINUS"){
                advance("MINUS");
                auto e1 = parse_primary_exp();
                return make_unique<UnaryExpression>("-", std::move(e1));
            }
            return parse_primary_exp();
        }

        unique_ptr<Expression> parse_primary_exp(){
            // <primary_exp> ::= <literal> | "(" <exp> ")"
            Token token = current_token;

            if (token.get_type() == "INTEGER" || token.get_type() == "BOOLEAN") {
                advance(token.get_type());

                auto value = token.get_value();

                return visit([](auto&& arg) -> unique_ptr<Expression> {
                    using T = decay_t<decltype(arg)>;
                    if constexpr (is_same_v<T, int> || is_same_v<T, bool>) {
                        return make_unique<PrimaryExpression>(make_unique<Literal>(arg));
                    } else {
                        throw ParserError("Tipo de Literal não suportado");
                    }
                }, value);
            }
            if (token.get_type() == "LPAREN"){
                advance("LPAREN");
                auto e1 = parse_exp();
                advance("RPAREN");
                return make_unique<PrimaryExpression>(std::move(e1));
            }
            error("Token inesperado: " + token.get_type());
            return nullptr;
        }
};

class ExpressionEvaluator {
    // Main class para processamento da expressão
    public:
        ExpressionEvaluator() {}
        ~ExpressionEvaluator() = default;

        static variant<int, bool> evaluate(const string& input_expression){
            if (input_expression.empty()){
                throw invalid_argument("Expressão vazia");
            }
            Lexer lexer(input_expression);
            Parser parser(lexer);
            return parser.evaluate();
        }
};

#endif