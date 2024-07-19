#include "Board.h"

int random_int(int min, int max) {
    std::random_device rd;
    std::mt19937 rand_gen(rd());
    std::uniform_int_distribution dist(min, max);
    return dist(rand_gen);
}

Cell::Cell(int x, int y, int x_bound, int y_bound, char type, std::mutex* board_mutex, CellMatrix* board_grid)
    : x(x), y(y), x_bound(x_bound), y_bound(y_bound), type(type), board_mutex(board_mutex), board_grid(board_grid), running(true) {
    next_x = x; 
    next_y = y; 
}

Cell::~Cell() {
    stop();
    if (cell_thread.joinable()) {
        cell_thread.join();
    }
}

void Cell::random_move() {
    int rng = random_int(0, 3);
    next_x = x;
    next_y = y; // Reiniciar para valores atuais

    switch (rng) {
        case 0:
            if (x > 0) next_x = x - 1;
            break;
        case 1:
            if (x < x_bound) next_x = x + 1;
            break;
        case 2:
            if (y > 0) next_y = y - 1;
            break;
        case 3:
            if (y < y_bound) next_y = y + 1;
            break;
        default:
            break;
    }
}

void Cell::thread_safe_move() {
    while (running.load()) { // Verifica a condição de término
        random_move();
        std::lock_guard<std::mutex> lock(*board_mutex);
        if (next_x >= 0 && next_x <= x_bound && next_y >= 0 && next_y <= y_bound) {
            if ((*board_grid)[next_x][next_y] == nullptr) {
                (*board_grid)[x][y] = nullptr; 
                x = next_x;
                y = next_y;
                (*board_grid)[x][y] = this;
                 // Move a célula para a nova posição
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Cell::start() {
    cell_thread = std::thread(&Cell::thread_safe_move, this);
}

void Cell::stop() {
    running.store(false);
}

Board::Board(int rows, int cols) : rows(rows), cols(cols) {
    grid = CellMatrix(rows, std::vector<Cell*>(cols, nullptr));

    // Preenchendo a primeira linha
    for (int j = 0; j < cols; ++j) {
        grid[0][j] = new Cell(0, j, rows - 1, cols - 1, 'b', &board_mutex, &grid);
    }
    // Preenchendo a última linha
    for (int j = 0; j < cols; ++j) {
        grid[rows - 1][j] = new Cell(rows - 1, j, rows - 1, cols - 1, 'r', &board_mutex, &grid);
    }
}

Board::~Board() {
    join_threads();
    for (auto& row : grid) {
        for (auto& elem : row) {
            delete elem;
        }
    }
}

void Board::print_grid() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != nullptr) {
                std::cout << grid[i][j]->type << " ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Board::start_threads() {
    for (auto& row : grid) {
        for (auto& elem : row) {
            if (elem != nullptr) {
                elem->start();
            }
        }
    }
}

void Board::join_threads() {
    for (auto& row : grid) {
        for (auto& elem : row) {
            if (elem != nullptr) {
                elem->stop();
                if (elem->cell_thread.joinable()) {
                    elem->cell_thread.join();
                }
            }
        }
    }
}
