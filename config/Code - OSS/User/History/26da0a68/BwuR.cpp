#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <complex>
#include "../include/apple.h"
#include "../include/snake.h"

using namespace std;


int main(){
    int rows = 30, cols = 50, direction = 1; 
    bool apple_good, snake_tile_good, snake_eat_himself;
    
    vector<complex<int>> snake; // Snake initialized
    snake.push_back(complex<int>(1, 1)); // set the starting point to 1, 1
    
    // Create the vector and initialize the values to 0.
    vector<vector<int>> matrix(rows, vector<int>(cols, 0));
    auto[random_num_col, random_num_row] = random_apple_index(rows, cols);
    matrix[random_num_row][random_num_col] = 1;
    
    while(true) {
        // Snake movement
        snake = snake_movement(snake, direction);
        
        // Handling Snake game tile limit
        snake_tile_good = check_snake_limit(snake, rows, cols);
        if (!snake_tile_good){
            cout << "GAME OVER | Exceed Tile limit !";
            break;
        }
        // Handling smake eat himself
        snake_eat_himself = check_snake_eat_himself(snake);
        if (snake_eat_himself){
            cout << "GAME OVER | Snake Ate himself !";
            break;
        }
        
        // Display the matrix
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                
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
        // Sleeps for 800 milliseconds
        this_thread::sleep_for(chrono::milliseconds(800));
        // Reset the apple index
        system("clear");
    }
    return 0;

}

