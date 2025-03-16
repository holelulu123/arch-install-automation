#include <iostream>
#include <fstream>
#include <string>

void write_file(std::string filename){
    std::ofstream File(filename);
    File << "First Line to write in the file" << std::endl; // endl would make the next write the next line
    File << "Second Line to write" << std::endl;
    File.close();
}


int main(){

    std::ofstream MyFile("filename.txt");
    MyFile << "First text to write to file" << std::endl;
    MyFile << "Second text to write";
    MyFile.close();

}
