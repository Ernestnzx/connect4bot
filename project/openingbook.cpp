#include "bitboard.hpp"
#include "solver.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <utility>
#include <unordered_map>
#include <vector>

int moves[] = {3,2,4,1,5,0,6};
std::map<uint64_t, std::string> state_tracker;
std::unordered_map<uint64_t, int> best_moves;

void generate_moves(std::string board_state, int depth, BitBoard &board) {
    if (depth <= 0) {
        state_tracker[board.get_key()] = board_state;
        return;
    }
    uint64_t next_moves = board.get_non_losing_moves();
    for (int i = 0; i < 7; i++) {
        if (uint64_t move_mask = board.get_move_mask(next_moves, i)) {
            BitBoard next_board(board);
            next_board.move(move_mask);
            generate_moves(board_state += i+'1', depth-1, next_board);
            board_state.pop_back();
        }
    }
}

std::pair<int,int> get_best_moves(uint64_t key, std::string board_state) {
    BitBoard board;
    Solver solver;
    board.move(board_state);
    int best_score = -1e9, best_move = -1;
    uint64_t next_moves = board.get_non_losing_moves();
    for (int m : moves) {
        if (uint64_t move_mask = board.get_move_mask(next_moves, m)) {
            BitBoard next_board(board);
            next_board.move(move_mask);
            int score = -solver.solve(next_board);
            if (best_score < score) {
                best_score = score;
                best_move = m;
            }
        }
    }
    assert(key == board.get_key());
    best_moves[board.get_key()] = best_move;
    return std::pair<int,int>(best_move, best_score);
}

int main() {
    BitBoard board;
    generate_moves("",7, board);
    std::cout << state_tracker.size() << '\n';
    uint64_t mask; std::string state, name;
    std::cin >> name;
    std::ofstream file("./"+name+".txt");
    while (std::cin >> mask >> state) {
        auto p = get_best_moves(mask, state);
        file << mask << ' ' << state << ' ' << p.first << ' ' << p.second << '\n';
    }
    file.close();
} 
