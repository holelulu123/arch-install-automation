#include <random>

using namespace std;

int main(){
    random_device rd;
    mt19937 gen(rd());
    
    uniform_real_distribution<> distrib_rows(0, rows);
    uniform_real_distribution<> distrib_cols(0, cols);


}