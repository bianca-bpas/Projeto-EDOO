#include "parser.h"
using namespace std;

int main(){
    int cases; cin >> cases;
    // Ignora a newline ao ler cases
    cin.ignore();

    ExpressionEvaluator evaluator;
    
    for (int i = 1; i <= cases; i++){
        string input; getline(cin, input);

        try{
            auto result = evaluator.evaluate(input);

            visit([](auto&& value) {
                using T = decay_t<decltype(value)>;
                if constexpr (is_same_v<T, int>) {cout << value;}
                else if constexpr(is_same_v<T, bool>) {cout << (value ? "true" : "false");}
                cout << '\n';
                
            }, result);
        } catch(exception&){
            cout << "error" << '\n';
        }   
    }
    return 0;
}