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

        inline string& get_type() { return this->type; }
        inline auto& get_value() { return this->value; }
        string to_string();
};

#endif