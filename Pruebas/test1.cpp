#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;


const int N = 100; // grid size
const int CELL_SIZE = 20; // cell size in pixels

int grid[N][N];
int next_grid[N][N];

// function to initialize the grid with a random pattern
void initialize_grid() {
    std::srand(std::time(nullptr));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = std::rand() % 2;
        }
    }
}

// function to compute the next state of the cells
void compute_next_state() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int num_neighbors = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    int row = (i + k + N) % N;
                    int col = (j + l + N) % N;
                    num_neighbors += grid[row][col];
                }
            }
            num_neighbors -= grid[i][j];
            if (grid[i][j] == 1 && (num_neighbors == 2 || num_neighbors == 3)) {
                next_grid[i][j] = 1;
            } else if (grid[i][j] == 0 && num_neighbors == 3) {
                next_grid[i][j] = 1;
            } else {
                next_grid[i][j] = 0;
            }
        }
    }
}

// function to update the grid with the next state of the cells
void update_grid() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = next_grid[i][j];
        }
    }
}


int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(N * CELL_SIZE, N * CELL_SIZE), "Conway's Game of Life");

    // create the cell shape
    sf::RectangleShape cell_shape(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    // initialize the grid
    initialize_grid();

    // game loop
    while (window.isOpen()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } 
            else if (event.type == sf::Event::MouseButtonPressed) {
                // get the cell coordinates from the mouse position
                int i = event.mouseButton.x / CELL_SIZE;
                int j = event.mouseButton.y / CELL_SIZE;

                // toggle the cell state
                grid[i][j] = (grid[i][j] == 0) ? 1 : 0;
            }
        }

        // draw the cells
        window.clear(sf::Color::White);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 1) {
                    cell_shape.setPosition(i * CELL_SIZE, j * CELL_SIZE);
                    cell_shape.setFillColor(sf::Color::Black);
                    window.draw(cell_shape);
                }
            }
        }

        // compute the next state of the cells
        compute_next_state();

        // update the grid with the next state of the cells
        update_grid();

        // display the window
        window.display();
        
        sleep(1);
    }

    return 0;
}
