#include <iostream>
#include <string>
class Car {
    public:
        std::string brand = "musti";
        void honk()
            std::cout << "Honk honk i am a " << brand << std::endl;
        }
};

class Vehicle: public Car {
    public:
        std::string model = "Mustang";
        
} 
int main(){
    Vehicle myobj("suzuku", 1922);
    return 0;
}