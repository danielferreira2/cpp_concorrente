#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <mutex>
#include <thread>
#include <atomic>

int random_int(int min, int max);

struct Cell {
    using CellMatrix = std::vector<std::vector<Cell*>>;
    char type;
    int x, y;
    int next_x, next_y;
    int x_bound, y_bound;
    std::mutex* board_mutex;
    CellMatrix* board_grid;
    std::thread cell_thread;
    std::atomic<bool> running;

    Cell(int x, int y, int x_bound, int y_bound, char type, std::mutex* board_mutex, CellMatrix* board_grid);
    ~Cell();

    void random_move();
    void thread_safe_move();
    void start();
    void stop();
};

class Board {
    using CellMatrix = std::vector<std::vector<Cell*>>;
private:
    int rows;
    int cols;
    CellMatrix grid;
    std::mutex board_mutex;

public:
    Board(int rows, int cols);
    ~Board();
    void print_grid();
    void start_threads();
    void join_threads();
};

#endif // BOARD_H
