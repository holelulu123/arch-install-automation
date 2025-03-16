#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <complex>
#include <cstdint>

using namespace std;


vector<complex<uint8_t>> snake_movement(vector<complex<uint8_t>> snake, uint8_t direction);

bool check_snake_limit(vector<complex<uint8_t>> snake, int rows, int cols);

#endif