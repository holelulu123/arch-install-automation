#include <iostream>
#include <fstream>

int main(){

    std::ofstream MyFile("filename.txt");
    MyFile << "First text to write to file";
    MyFile << "Second text to write";
    MyFile.close();

}
