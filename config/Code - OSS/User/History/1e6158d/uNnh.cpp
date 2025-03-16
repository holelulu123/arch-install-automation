#include <complex>
#include <vector>
#include <iostream>

using namespace std;

// snake direction | left -> 0 | right -> 1 | down -> 2 | up -> 3 | 
vector<complex<int>> increase_snake(vector<complex<int>> snake, int direction){
    int snake_size = snake.size();
    int real_1 = snake[snake_size - 1].real();
    int imag_1 = snake[snake_size - 1].imag();

    if (snake_size == 1){
        if (direction == 0){
            snake.push_back(complex<int>(real_1 + 1, imag_1));
        }
        else if (direction == 1){
            snake.push_back(complex<int>(real_1 - 1, imag_1));
        }
        else if (direction == 2){
            snake.push_back(complex<int>(real_1, imag_1 - 1));
        }
        else if (direction == 3){
            snake.push_back(complex<int>(real_1, imag_1 + 1));
        }
    }
    else if (snake_size > 1) {

        int real_2 = snake[snake_size - 2].real();
        int imag_2 = snake[snake_size - 2].imag();
        int delta_real = real_1 - real_2; 
        int delta_imag = imag_1 - imag_2; 
        if (delta_real == 0 and delta_imag == 1){
            snake.push_back(complex<int>(real_1, imag_1 + 1));
        }
        else if (delta_real == 0 and delta_imag == - 1){
            snake.push_back(complex<int>(real_1, imag_1 - 1));
        }
        else if (delta_real == 1 and delta_imag == 0){
            snake.push_back(complex<int>(real_1 + 1, imag_1));

        }
        else if (delta_real == -1 and delta_imag == 0){
            snake.push_back(complex<int>(real_1 - 1, imag_1));

        }
    }
    return snake;
}   

vector<complex<int>> snake_movement(vector<complex<int>> snake, int direction){
    cout << "snake size is: " << snake.size() << "\n";
    if (snake.size() > 1) {
        for (int i = 1; i < snake.size(); i ++) {
            snake[i] = snake[i - 1];
        }
    }
    int last_real = snake[0].real(); 
    int last_imag = snake[0].imag();
    cout << "Real is: " << last_real << " | " << "Imag is: " << last_imag << "\n";
    if (direction == 0) {
            snake[0] = complex<int>(last_real - 1, last_imag); // left
    }
    else if (direction == 1){
            snake[0] = complex<int>(last_real + 1, last_imag); // right 
    }
    else if (direction == 2){
            snake[0] = complex<int>(last_real, last_imag + 1); // down
    }
    else if (direction == 3){
            snake[0] = complex<int>(last_real, last_imag - 1); // up
    }
    return snake;
}

bool check_snake_limit(vector<complex<int>> snake, int rows, int cols){
    int real = snake[0].real();
    int imag = snake[0].imag();
    if (real > cols - 1 or real < 0){
        return false;
    }
    else if(imag > rows - 1 or imag < 0){
        return false;
    }
    else {
        return true;
    }

}

bool check_snake_eat_himself(vector<complex<int>> snake){
    for (int i = 0; i < snake.size(); i++) {
        for(int j = 0; j < snake.size(); j++){
            if (i!=j){
                if (snake[j].real() == snake[i].real() and snake[j].imag() == snake[i].imag()){
                    return true;
                }
            }
        }
    }
    return false;
} 

bool check_eat_apple(vector<complex<int>> snake, int apple_row, int apple_col){
    /* Check here if the the the snake is standing on an apple, if so, he gets bigger 
    else the game continue normally
    */
    bool flag_eat_apple = false;
    for (int i = 0; i < snake.size(); i++){
        if (snake[i].real() == apple_col and snake[i].imag() == apple_row){
            flag_eat_apple = true;
            break;
        }
    }
    return flag_eat_apple;
    
}
vector<vector<int>> snake_on_matrix(vector<complex<int>> snake, vector<vector<int>> matrix){
    /*
    This function marks the matrix in the right indexes where snakes located
    
    */
    int real, imag;
    for (int i = 0; i < matrix.size(); i++){
        for (int j = 0; j < matrix[1].size(); j++){
            if (matrix[i][j] == 2){
                matrix[i][j] = 0;
            }
        }
    }
    for (int x = 0; x < snake.size(); x++){
        real = snake[x].real();
        imag = snake[x].imag();
        matrix[imag][real] = 2;
    }
    return matrix;

}

