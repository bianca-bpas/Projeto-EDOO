#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "expressions.h"
#include <map>

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
            Expression* expr = parse_exp();
            return expr->evaluate();
        }

        Expression* parse_exp(){
            // <exp> ::= <or_exp>
            return parse_or_exp();
        }

        Expression* parse_or_exp(){
            // <or_exp> ::= <and_exp> [ "||" <and_exp> ]
            Expression* e1;
            e1 = parse_and_exp();

            if (current_token.get_type() == "OR"){
                advance("OR");
                Expression* e2;
                e2 = parse_and_exp();
                return new BinaryExpression(e1, "||", e2);
            } else {
                return e1;
            }
        }

        Expression* parse_and_exp(){
            // <and_exp> ::= <eq_exp> [ "&&" <eq_exp> ]
            Expression* e1;
            e1 = parse_eq_exp();

            if (current_token.get_type() == "AND"){
                advance("AND");
                Expression* e2;
                e2 = parse_eq_exp();
                return new BinaryExpression(e1, "&&", e2);
            } else {
                return e1;
            }
        }

        Expression* parse_eq_exp(){
            // <eq_exp> ::= <rel_exp> [ "==" <rel_exp> | "!=" <rel_exp> ]
            Expression* e1;
            e1 = parse_rel_exp();

            map<string, string> operadores = {
                {"EQUALS","=="},
                {"NOT_EQUALS","!="}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                Expression* e2;
                e2 = parse_rel_exp();
                return new BinaryExpression(e1, operador, e2);
            } else {
                return e1;
            }
        }

        Expression* parse_rel_exp(){
            // <rel_exp> ::= <add_exp> [ "<" <add_exp> | ">" <add_exp> | "<=" <add_exp> | ">=" <add_exp> ]
            Expression* e1;
            e1 = parse_add_exp();

            map<string, string> operadores = {
                {"LESS","<"},
                {"GREATER",">"},
                {"LESS_EQUAL","<="},
                {"GREATER_EQUAL", ">="}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                Expression* e2;
                e2 = parse_add_exp();
                return new BinaryExpression(e1, operador, e2);
            } else {
                return e1;
            }
        }
        
        Expression* parse_add_exp(){
            // <add_exp> ::= <mul_exp> [ "+" <mul_exp> | "-" <mul_exp> ]
            Expression* e1;
            e1 = parse_mul_exp();

            map<string, string> operadores = {
                {"PLUS","+"},
                {"MINUS","-"}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                Expression* e2;
                e2 = parse_mul_exp();
                return new BinaryExpression(e1, operador, e2);
            } else {
                return e1;
            }
        }

        Expression* parse_mul_exp(){
            // <mul_exp> ::= <unary_exp> [ "*" <unary_exp> | "/" <unary_exp> ]
            Expression* e1;
            e1 = parse_unary_exp();

            map<string, string> operadores = {
                {"MULTIPLY","*"},
                {"DIVIDE","/"}
            };

            if (operadores.find(current_token.get_type()) != operadores.end()){
                string operador = operadores[current_token.get_type()];
                advance(current_token.get_type());

                Expression* e2;
                e2 = parse_unary_exp();
                return new BinaryExpression(e1, operador, e2);
            } else {
                return e1;
            }
        }

        Expression* parse_unary_exp(){
            // <unary_exp> ::= "-" <primary_exp> | <primary_exp>
            Expression* e1;
            if (current_token.get_type() == "MINUS"){
                advance("MINUS");

                e1 = parse_primary_exp();
                return new UnaryExpression("-", e1);
            }
            e1 = parse_primary_exp();
            return e1;
        }

        Expression* parse_primary_exp(){
            // <primary_exp> ::= <literal> | "(" <exp> ")"
            Token token = current_token;

            if (token.get_type() == "INTEGER" || token.get_type() == "BOOLEAN") {
                advance(token.get_type());

                auto value = token.get_value();

                return visit([](auto&& arg) -> Expression* {
                    using T = decay_t<decltype(arg)>;
                    if constexpr (is_same_v<T, int> || is_same_v<T, bool>) {
                        return new PrimaryExpression(new Literal(arg));
                    } else {
                        throw ParserError("Tipo de Literal não suportado");
                    }
                }, value);
            }
            if (token.get_type() == "LPAREN"){
                advance("LPAREN");
                Expression* e1;
                e1 = parse_exp();
                advance("RPAREN");
                return new PrimaryExpression(e1);
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