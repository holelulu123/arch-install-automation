#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <complex>
#include <cstdint>

using namespace std;


vector<complex<int>> snake_movement(vector<complex<int>> snake, int direction);

bool check_snake_limit(vector<complex<int>> snake, int rows, int cols);

bool check_snake_eat_himself(vector<complex<int>> snake);

#endif