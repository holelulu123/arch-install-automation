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
    int total_num_samps = 100000;
    int rx_count = 0; 
    while(rx_count < total_num_samps){
        for (int i = 0; i < buffer; i++){
            vector.push_back((i,i+1));
        }
        if(outfiles[0].is_open()){
            
        }
        rx_count += buffer
    }    

    return 0;
}