#include <iostream>

class Myclass {
    public:
        Myclass(){
            std::cout << "Hello world" << std::endl;
        }
};
int main(){
    Myclass myobj;
    return 0;
}