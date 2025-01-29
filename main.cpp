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

            if (holds_alternative<int>(result)) {
                cout << get<int>(result);
            }
            else if (holds_alternative<bool>(result)) {
                cout << (get<bool>(result) ? "true" : "false");
            }
            cout << '\n';

        } catch(exception&){
            cout << "error" << '\n';
        }   
    }
    return 0;
}