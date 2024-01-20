#include "bitboard.hpp"
#include "solver.hpp"

#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <utility>
#include <unordered_map>
#include <vector>

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
    BitBoard board; Solver solver;
    board.move(board_state);
    int best_score = -1e9, best_move = -1;
    uint64_t next_moves = board.get_non_losing_moves();
    if (next_moves == 0) {
        return {0, board.get_losing_score()};
    }
    for (int i = 0; i < 7; i++) {
        if (uint64_t move_mask = board.get_move_mask(next_moves, i)) {
            BitBoard next_board(board);
            next_board.move(move_mask);
            int score = -solver.solve(next_board);
            if (best_score < score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    assert(key == board.get_key());
    best_moves[board.get_key()] = best_move;
    printf("%lu %s %d %d\n", key, board_state.c_str(), best_move, best_score);
    return std::pair<int,int>(best_move, best_score);
}

int main() {
    BitBoard board;
    generate_moves("",9,board);
    std::cout << state_tracker.size() << '\n';
    std::ofstream output("../opening_book/10_ply_book.txt");
    std::vector<std::future<std::tuple<uint64_t,std::string,int,int>>> futures;
    std::vector<std::tuple<uint64_t,std::string,int,int>> book; 
    book.reserve(state_tracker.size());
    for (auto it = state_tracker.begin(); it != state_tracker.end(); it++) {
        uint64_t mask = it->first; std::string state = it->second;
        futures.push_back(std::async(std::launch::async, [mask, state]() 
                -> std::tuple<uint64_t,std::string,int,int> {
            auto result = get_best_moves(mask, state);
            return {mask, state, result.first, result.second};
        }));
        if (futures.size() == 200 || it == --state_tracker.end()) {
            for (auto &future : futures) {
                book.push_back(future.get());
            }
            futures.clear();
        }
    }
    sort(book.begin(), book.end());
    for (auto &[mask, state, move, score] : book) {
        output << mask << ' ' << state << ' ' << move << ' ' << score << '\n';
    }
    output.close();
} 
