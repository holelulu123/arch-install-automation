#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>

int main(){
    std::ofstream outfiles;
    std::string filename;
    std::vector<std::complex<float>> my_vector;  
    filename = "node_" + std::to_string(0);
    std::cout << "The filename is: " << filename << std::endl;
    outfiles.open(filename, std::ofstream::binary);
    int buffer = 2000;
    int total_num_samps = 100000;
    int rx_count = 0; 
    while(rx_count < total_num_samps){
        for (int i = 0; i < buffer; i++){
            my_vector.push_back(std::complex<float>(i,i+1));
        }
        if(outfiles.is_open()){
            outfiles.write((const char*)my_vector);
        }
        rx_count += buffer
    }    

    return 0;
}