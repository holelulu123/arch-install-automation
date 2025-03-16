#include <random>

using namespace std;

int random_apple_index(int row, int col){
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> distrib_rows(0, row);
    uniform_real_distribution<> distrib_cols(0, col);
    random_num_col = distrib_cols(gen);
    random_num_row = distrib_rows(gen);

}
