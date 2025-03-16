#include <random>
#include <utility>

using namespace std;

pair<int, int> random_apple_index(int row, int col){
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> distrib_rows(0, row);
    uniform_real_distribution<> distrib_cols(0, col);
    int random_num_col = distrib_cols(gen);
    int random_num_row = distrib_rows(gen);

    return {random_num_col, random_num_row};
}

