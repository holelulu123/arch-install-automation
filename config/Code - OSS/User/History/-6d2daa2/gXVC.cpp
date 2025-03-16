#include <iostream>
#include <fstream>
#include <string>

void write_file(std::string filename){
    std::ofstream File(filename);
}


int main(){

    std::ofstream MyFile("filename.txt");
    MyFile << "First text to write to file" << std::endl;
    MyFile << "Second text to write";
    MyFile.close();

}
