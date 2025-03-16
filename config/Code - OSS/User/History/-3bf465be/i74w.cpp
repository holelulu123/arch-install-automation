#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>
int main(){
    std::vector<std::ofstream> outfiles(2);
    std::string filename;
    for (int i = 0; i < outfiles.size(); i++){
        filename = "node_" + i;
        std::cout << "The filename is: " << filename << std::endl;
        outfiles[i].open(filename, std::ofstream::binary);
    }
    return 0;
}