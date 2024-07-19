
#include "Board.h"

int main(){


    Board *b = new Board(10,10);
    b->start_threads();
    while(true){
        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        b->print_grid();
    }   



    return 0;
}
