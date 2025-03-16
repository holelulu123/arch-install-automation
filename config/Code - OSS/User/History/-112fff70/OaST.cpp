#include <complex>
#include <vector>
#include <cstdint>
#include <iostream>

using namespace std;
// snake direction | left -> 0 | right -> 1 | down -> 2 | up -> 3 | 
vector<complex<int>> snake_movement(vector<complex<int>> snake, int direction){
    if (snake.size() > 1) {
        
        for (int i = 1; i > snake.size(); i ++) {
            snake[i] = snake[i - 1];
        }
    }
    int last_real = snake[0].real(); 
    int last_imag = snake[0].imag();
    cout << "Direction is: " << direction << "\n";
    cout << "Real is: " << last_real << " | " << "Imag is: " << last_imag << "\n";
    if (direction == 0) {
            snake[0] = complex<int>(last_real - 1, last_imag); // left
    }
    else if (direction == 0){
            snake[0] = complex<int>(last_real + 1, last_imag); // right 
    }
    else if (direction == 0){
            snake[0] = complex<int>(last_real, last_imag + 1); // down
    }
    else if (direction == 0){
            snake[0] = complex<int>(last_real, last_imag - 1); // up
    }
    return snake;
}
bool check_snake_limit(vector<complex<int>> snake, int rows, int cols){
    int real = snake[0].real();
    int imag = snake[0].imag();
    if (real > rows or real < 0){
        return false;
    }
    else if(imag > cols or imag < 0){
        return false;
    }
    else {
        return true;
    }
}  

