#include "token.h"

// Para debug
string Token::to_string() {
    string result = "Token(" + type + ", ";

    if (holds_alternative<int>(value)) {
        result += std::to_string(get<int>(value));
    } else if (holds_alternative<bool>(value)) {
        result += get<bool>(value) ? "true" : "false";
    } else {
        result += get<string>(value);
    }
    result += ")";
    
    return result;
}
