#ifndef APPLE_H
#define APPLE_H

#include <vector>
#include <complex>
#include <utility>
using namespace std;

// Function declaration

pair<int, int> random_apple_index(int row, int col);
bool check_apple_coordination(vector<complex<int>> snake, int apple_row, int apple_col);

#endif 