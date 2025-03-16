#include <iostream>
#include <fstream>
#include <string>

void write_file(std::string filename){
    std::ofstream File(filename);
    File << "First Line to write in the file" << std::endl; // endl would make the next write the next line
    File << "Second Line to write" << std::endl;
    File.close();
}

void read_file(std::string filename){
    std::string output_text;
    std::ifstream File(filename);
    while(getline (File, output_text)){
        std::cout << output_text << std::endl;
    }
    File.close();
}

int main(){
    std::string filename = "myfile.txt";
    write_file(filename);
    read_file(filename);
    return 0;
}
