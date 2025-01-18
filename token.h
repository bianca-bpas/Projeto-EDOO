#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <variant>
using namespace std;

class Token {
    private:
        string type;
        variant <int, bool, string> value;
    
    public:
        Token(string t, variant<int, bool, string> v) : type(t), value(v) {}
        ~Token() = default;

        // Getters
        const string& get_type() const { return this->type; }
        const auto& get_value() const { return this->value; }
        
        // Para debugger
        string to_string() const {
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
};

#endif