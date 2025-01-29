#include "parser.h"

void Parser::error(const string& message) {
    throw ParserError("Erro de sintaxe: " + message);
}

void Parser::advance(const string& expected_type) {
    if (current_token.get_type() != expected_type) {
        error("Esperado " + expected_type + ", obteve " + current_token.get_type());
    }
    current_token = lexer.get_next_token();
}

variant<int, bool> Parser::evaluate() {
    auto expr = parse_exp();
    return expr->evaluate();
}

unique_ptr<Expression> Parser::parse_exp() {
    return parse_or_exp();
}

unique_ptr<Expression> Parser::parse_or_exp() {
    auto e1 = parse_and_exp();

    if (current_token.get_type() == "OR") {
        advance("OR");
        auto e2 = parse_and_exp();
        return make_unique<BinaryExpression>(move(e1), "||", move(e2));
    }
    return e1;
}

unique_ptr<Expression> Parser::parse_and_exp() {
    auto e1 = parse_eq_exp();

    if (current_token.get_type() == "AND") {
        advance("AND");
        auto e2 = parse_eq_exp();
        return make_unique<BinaryExpression>(move(e1), "&&", move(e2));
    }
    return e1;
}

unique_ptr<Expression> Parser::parse_eq_exp() {
    auto e1 = parse_rel_exp();

    map<string, string> operadores = {
        {"EQUALS", "=="},
        {"NOT_EQUALS", "!="}
    };

    if (operadores.find(current_token.get_type()) != operadores.end()) {
        string operador = operadores[current_token.get_type()];
        advance(current_token.get_type());

        auto e2 = parse_rel_exp();
        return make_unique<BinaryExpression>(move(e1), operador, move(e2));
    }
    return e1;
}

unique_ptr<Expression> Parser::parse_rel_exp() {
    auto e1 = parse_add_exp();

    map<string, string> operadores = {
        {"LESS", "<"},
        {"GREATER", ">"},
        {"LESS_EQUAL", "<="},
        {"GREATER_EQUAL", ">="}
    };

    if (operadores.find(current_token.get_type()) != operadores.end()) {
        string operador = operadores[current_token.get_type()];
        advance(current_token.get_type());

        auto e2 = parse_add_exp();
        return make_unique<BinaryExpression>(move(e1), operador, move(e2));
    }
    return e1;
}

unique_ptr<Expression> Parser::parse_add_exp() {
    auto e1 = parse_mul_exp();

    map<string, string> operadores = {
        {"PLUS", "+"},
        {"MINUS", "-"}
    };

    if (operadores.find(current_token.get_type()) != operadores.end()) {
        string operador = operadores[current_token.get_type()];
        advance(current_token.get_type());

        auto e2 = parse_mul_exp();
        return make_unique<BinaryExpression>(move(e1), operador, move(e2));
    }
    return e1;
}

unique_ptr<Expression> Parser::parse_mul_exp() {
    auto e1 = parse_unary_exp();

    map<string, string> operadores = {
        {"MULTIPLY", "*"},
        {"DIVIDE", "/"}
    };

    if (operadores.find(current_token.get_type()) != operadores.end()) {
        string operador = operadores[current_token.get_type()];
        advance(current_token.get_type());

        auto e2 = parse_unary_exp();
        return make_unique<BinaryExpression>(move(e1), operador, move(e2));
    }
    return e1;
}

unique_ptr<Expression> Parser::parse_unary_exp() {
    if (current_token.get_type() == "MINUS") {
        advance("MINUS");
        auto e1 = parse_primary_exp();
        return make_unique<UnaryExpression>("-", move(e1));
    }
    return parse_primary_exp();
}

unique_ptr<Expression> Parser::parse_primary_exp() {
    Token token = current_token;

    if (token.get_type() == "INTEGER" || token.get_type() == "BOOLEAN") {
        advance(token.get_type());
        auto value = token.get_value();

        if (holds_alternative<int>(value)) {
            return make_unique<PrimaryExpression>(make_unique<Literal>(get<int>(value)));
        } 
        else if (holds_alternative<bool>(value)) {
            return make_unique<PrimaryExpression>(make_unique<Literal>(get<bool>(value)));
        }
        throw ParserError("Tipo de Literal não suportado");
    }

    if (token.get_type() == "LPAREN") {
        advance("LPAREN");
        auto e1 = parse_exp();
        advance("RPAREN");
        return make_unique<PrimaryExpression>(move(e1));
    }

    error("Token inesperado: " + token.get_type());
    return nullptr;
}

variant<int, bool> ExpressionEvaluator::evaluate(const string& input_expression) {
    if (input_expression.empty()) {
        throw invalid_argument("Expressão vazia");
    }
    Lexer lexer(input_expression);
    Parser parser(lexer);
    return parser.evaluate();
}
