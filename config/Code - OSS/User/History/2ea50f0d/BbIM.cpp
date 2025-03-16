#include <iostream>

class Myclass {
    public:
void myMethod(){
    std::cout << "Hello world" << std::endl;
}

};

int main(){
    Myclass myobj;
    myobj.myMethod();
    return 0;
}