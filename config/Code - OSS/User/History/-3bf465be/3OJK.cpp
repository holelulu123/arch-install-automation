#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>
int main(){
    std::vector<std::ofstream> outfiles(1);
    std::string filename;
    filename = "node_" + std::to_string(0);
    std::cout << "The filename is: " << filename << std::endl;
    outfiles[0].open(filename, std::ofstream::binary);
    for
    return 0;
}