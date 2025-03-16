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
    Car myobj('Suzuki', 2016);
    return 0;
}