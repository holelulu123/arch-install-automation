#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <complex>
#include "../include/apple.h"
#include "../include/snake.h"
#include "../include/keyboard.h"

using namespace std;
char last_valid_key = 'd';

int main(){
    thread keyListener(getKey);

    int rows = 30, cols = 50, direction = 1; 
    bool apple_good=false, snake_tile_good, snake_eat_himself, snake_eat_apple;
    
    vector<complex<int>> snake; // Snake initialized
    snake.push_back(complex<int>(1, 1)); // set the starting point to 1, 1
    
    // Create the vector and initialize the values to 0.
    vector<vector<int>> matrix(rows, vector<int>(cols, 0));
    pair<int, int> apple_pair = random_apple_index(rows, cols);
    matrix[apple_pair.second][apple_pair.first] = 1;
    
    while(true) {
        direction = Keyboard_To_Direction(direction);
        cout << "Direction is: " << direction << '\n';
        // Snake movement
        snake = snake_movement(snake, direction);
        
        // Handling Snake game tile limit
        snake_tile_good = check_snake_limit(snake, rows, cols);
        if (!snake_tile_good){
            cout << "GAME OVER | Exceed Tile limit ! \n";
            break;
        }
        // Handling snake eat himself
        snake_eat_himself = check_snake_eat_himself(snake);
        if (snake_eat_himself){
            cout << "GAME OVER | Snake Ate himself ! \n";
            break;
        }
        snake_eat_apple = check_eat_apple(snake, apple_pair.second, apple_pair.first);
        if (snake_eat_apple){
            // Delete the last apple
            matrix[apple_pair.second][apple_pair.first] = 0;
            // Generate new apple coordinates
            while (!apple_good){
                apple_pair = random_apple_index(rows, cols);
                apple_good = check_apple_coordination(snake, apple_pair.second, apple_pair.first);
            }
            matrix[apple_pair.second][apple_pair.first] = 1;
            snake = increase_snake(snake, direction);
            cout << "Bon Appétit \n";
        }
        matrix = snake_on_matrix(snake, matrix);
        cout << "New apple col: " << apple_pair.first << " New apple row: " << apple_pair.second << "\n"; 
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
        // Sleeps for 200 milliseconds
        this_thread::sleep_for(chrono::milliseconds(200));
        // Reset the apple index
        system("clear");
        apple_good = false;
    }

    keyListener.join(); // Wait for the key listener thread to finish
    return 0;

}

