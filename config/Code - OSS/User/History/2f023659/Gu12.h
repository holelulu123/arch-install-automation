#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <complex>
#include <cstdint>

using namespace std;


vector<complex<int>> snake_movement(vector<complex<int>> snake, int direction);
vector<complex<int>> snake_eat_apple(vector<complex<int>>snake , int apple_row, int apple_col);
vector<complex<int>> increase_snake(vector<complex<int>> snake, int direction);

bool check_snake_limit(vector<complex<int>> snake, int rows, int cols);
bool check_snake_eat_himself(vector<complex<int>> snake);


#endif