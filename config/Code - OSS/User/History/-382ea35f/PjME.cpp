#include <iostream>
using namespace std;

void keyboard_input(){
    char input;
    std::cout << "Press a key \n";
    cin >> input;
    cout << "Your pressed " << input << "\n";

}
int main(){
    while(true){
        keyboard_input();
    }
    
}