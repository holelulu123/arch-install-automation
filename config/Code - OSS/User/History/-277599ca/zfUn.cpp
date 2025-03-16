#include <iostream>

int main(){
    int age;
    std::cout << "Enter Your age: " << std::endl;
    std::cin >> age;
    try{
        if (age >= 18){
            std::cout << "You can legally buy alcohol." << std::endl;
        }
        else{
            throw 100;
        }
    }

}