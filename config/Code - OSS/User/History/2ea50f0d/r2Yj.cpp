#include <iostream>
#include <string>
class Car {
    public:
        int year;
        string brand;
        Car(string brand, int year){
            std::cout << "Hello world" << std::endl;
        }
};
int main(){
    Myclass myobj;
    return 0;
}