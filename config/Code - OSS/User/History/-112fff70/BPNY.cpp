#include <complex>
#include <vector>
#include <iostream>

using namespace std;

// snake direction | left -> 0 | right -> 1 | down -> 2 | up -> 3 | 
vector<complex<int>> increase_snake(vector<complex<int>> snake, int direction){
    int snake_size = snake.size();
    int last_real = snake[snake_size - 1].real();
    int last_imag = snake[snake_size - 1].imag();

    if (direction == 0){
        snake.push_back(complex<int>(1, 1)); // set the starting point to 1, 1
    }
    else if (direction == 1){
        snake.push_back(complex<int>(1, 1)); // set the starting point to 1, 1
    }
    else if (direction == 2){
        snake.push_back(complex<int>(1, 1)); // set the starting point to 1, 1
    }
    else if (direction == 3){
        snake.push_back(complex<int>(1, 1)); // set the starting point to 1, 1
    }
    return snake;
}   

vector<complex<int>> snake_movement(vector<complex<int>> snake, int direction){
    if (snake.size() > 1) {
        
        for (int i = 1; i > snake.size(); i ++) {
            snake[i] = snake[i - 1];
        }
    }
    int last_real = snake[0].real(); 
    int last_imag = snake[0].imag();
    cout << "Real is: " << last_real << " | " << "Imag is: " << last_imag << "\n";
    if (direction == 0) {
            // cout << "Entered the first option. \n";
            snake[0] = complex<int>(last_real - 1, last_imag); // left
    }
    else if (direction == 1){
            // cout << "Entered the second option. \n";
            snake[0] = complex<int>(last_real + 1, last_imag); // right 
    }
    else if (direction == 2){
            // cout << "Entered the third option. \n";
            snake[0] = complex<int>(last_real, last_imag + 1); // down
    }
    else if (direction == 3){
            // cout << "Entered the fourth option. \n";
            snake[0] = complex<int>(last_real, last_imag - 1); // up
    }
    return snake;
}

bool check_snake_limit(vector<complex<int>> snake, int rows, int cols){
    int real = snake[0].real();
    int imag = snake[0].imag();
    if (real > cols or real < 0){
        return false;
    }
    else if(imag > rows or imag < 0){
        return false;
    }
    else {
        return true;
    }

}

bool check_snake_eat_himself(vector<complex<int>> snake){
    for (int i = 0; i < snake.size(); i++) {
        for(int j = 0; i < snake.size(); j++){
            if (snake[j].real() == snake[i].real() and snake[j].imag() == snake[i].imag()){
                return true;
            }
        }
    }
    return false;
} 

vector<complex<int>> snake_eat_apple(vector<complex<int>> snake, int apple_row, int apple_col, int direction){
    /* Check here if the the the snake is standing on an apple, if so, he gets bigger 
    else the game continue normally
    */
    bool eat_apple = false;
    for (int i = 0; i < snake.size(); i++){
        if (snake[i].real() == apple_col and snake[i].imag() == apple_row){
            eat_apple = true;
            break;
        }
    }
    
}

