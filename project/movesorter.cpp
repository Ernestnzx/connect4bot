#include "movesorter.hpp"

MoveSorter::MoveSorter() {
    num_moves = 0;
}

// note that the move order must be iterated backwards
void MoveSorter::add(uint64_t move, int score) {
    int pos = ++num_moves;
    while (--pos && sorted_moves[pos-1].second > score) 
        sorted_moves[pos] = sorted_moves[pos-1];
    sorted_moves[pos] = {move, score};
}

uint64_t MoveSorter::get() {
    return num_moves ? sorted_moves[--num_moves].first : 0;
}
