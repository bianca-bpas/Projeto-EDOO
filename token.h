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

        string& get_type() { return this->type; }
        auto& get_value() { return this->value; }
        string to_string();
};

#endif