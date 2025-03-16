#include <iostream>
#include <string>
class Car {
    public:
        int year;
        std::string brand;
        
        Car(std::string brand, int year){
            brand = brand;
            year = year;
        }
};
int main(){
    Car myobj("Suzuki", 2016);
    std::cout << "Brand: " << myobj.brand << std::endl;
    std::cout << "Year: " << myobj.year << std::endl;
    return 0;
}