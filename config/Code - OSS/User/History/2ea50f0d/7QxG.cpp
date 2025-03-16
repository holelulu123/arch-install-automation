#include <iostream>
#include <ostream>
#include <string>

class Car {
    public:
        std::string brand = "musti";
        void honk(){
            std::cout << "Honk honk i am a " << brand << std::endl;
        }
};

class Vehicle: public Car {
    public:
        Vehicle() {
            std::cout << "Hi" << std::endl;
        }
};

int main(){
    Vehicle myobj();
    return 0;
}