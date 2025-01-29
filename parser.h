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

        void error(const string& message);
        void advance(const string& expected_type);

    public:
        explicit Parser(const Lexer& l) : lexer(l), current_token(lexer.get_next_token()) {}
        ~Parser() = default;

        variant<int, bool> evaluate();
        unique_ptr<Expression> parse_exp();
        unique_ptr<Expression> parse_or_exp();
        unique_ptr<Expression> parse_and_exp();
        unique_ptr<Expression> parse_eq_exp();
        unique_ptr<Expression> parse_rel_exp();
        unique_ptr<Expression> parse_add_exp();
        unique_ptr<Expression> parse_mul_exp();
        unique_ptr<Expression> parse_unary_exp();
        unique_ptr<Expression> parse_primary_exp();
};

class ExpressionEvaluator {
    public:
        ExpressionEvaluator() {}
        ~ExpressionEvaluator() = default;
        static variant<int, bool> evaluate(const string& input_expression);
};

#endif