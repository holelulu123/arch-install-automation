#include <iostream>
#include <string>
class Car {
    public:
        int year;
        std::string brand;
        
        Car(std::string brand1, int year2){
            brand = brand1;
            year = year2;
        }
};

class Vehicle: public Car {
    public:
        string model = "Mustang";
} 
int main(){
    Car myobj("Suzuki", 2016);
    std::cout << "Brand: " << myobj.brand << std::endl;
    std::cout << "Year: " << myobj.year << std::endl;
    return 0;
}