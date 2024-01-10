#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "bitboard.hpp"
#include "movesorter.hpp"
#include "transposition.hpp"
#include <algorithm>

class Solver {
    private:
        // attributes
        int move_order[7] = {0,6,1,5,2,4,3};
        uint64_t explored;
        Transposition table;

    public:
        Solver();
        int solve(BitBoard &board);
        int negamax(BitBoard &board, int alpha, int beta);
        void reset();
        unsigned long long num_nodes();
};

#endif
