#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>
int main(){
    std::vector<std::ofstream> outfiles(1);
    std::string filename;
    std::vector<std::complex<float>> vector;  
    filename = "node_" + std::to_string(0);
    std::cout << "The filename is: " << filename << std::endl;
    outfiles[0].open(filename, std::ofstream::binary);
    int buffer = 2000;
    for (int i = 0; i < size; i ++){
        vector.push_back((i, i+1));
    }

    return 0;
}