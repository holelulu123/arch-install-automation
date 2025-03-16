#include <iostream>
#include <vector>
#include <cstdint>
#include <thread>
#include <chrono>
#include <complex>
#include "../include/apple.h"
#include "../include/snake.h"

using namespace std;


int main(){
    int random_num_col, random_num_row;
    int rows, cols;
    bool apple_good, snake_good;
    
    uint8_t direction = 1; // direction of snake initialization
    vector<complex<uint8_t>> snake; // Snake initialized
    snake.push_back(complex<uint8_t>(1, 1)); // set the starting point to 1, 1
    rows = 30; 
    cols = 50;
    // Create the vector and initialize the values to 0.
    vector<vector<uint8_t>> matrix(rows, vector<uint8_t>(cols, 0));
    while(true) {
        snake = snake_movement(snake, direction);
        
        // Generates Random row and col indexes for apple
        
        while (!apple_good){
            auto [random_num_col, random_num_row] = random_apple_index(rows, cols);
            // generating apple up until the index is good
        }
        // matrix[][] = {0}
        matrix[random_num_row][random_num_col] = 1;
        // Display the matrix
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                
                for (int x = 0; x < snake.size(); x++){
                    if (snake[x].real() == j and snake[x].imag() == i){
                        matrix[i][j] = 2;
                    }
                }
                if (matrix[i][j] == 0) { //if it's normal color show normal *  
                    cout <<  "\033[32m*\033[0m";
                }
                else if (matrix[i][j] == 1) { //print the apple with a different color
                    cout <<  "\033[91m*\033[0m";
                }
                else if (matrix[i][j] == 2) { // print the snake with a different color
                    cout << "\033[33m*\033[0m";
                }
            }
            cout << '\n';
        }
        // Sleeps for 500 milliseconds
        this_thread::sleep_for(chrono::milliseconds(800));
        // Reset the apple index
        matrix[random_num_row][random_num_col] = 0;
        system("clear");
    }
    return 0;

}

