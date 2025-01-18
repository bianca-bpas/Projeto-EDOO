#include <iostream>
#include <variant>
#include "token.h"
using namespace std;

int main() {
    string s = "2+4";

    Token t("INTEGER", "2");
    cout << t.to_string() << " ";

    return 0;
}