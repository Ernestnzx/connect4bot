#include "bitboard.hpp"
#include "solver.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>

std::map<uint64_t, std::tuple<std::string, int, int>> book;

int generate_book(int depth, BitBoard &board, std::string board_state) {
    if (depth <= 0) {
        assert(book.find(board.get_key()) != book.end());
        return std::get<2>(book[board.get_key()]);
    }
    int best_score = -1e9, best_move = -1;
    uint64_t next_mask = board.get_non_losing_moves();
    if (next_mask == 0) {
        return board.get_losing_score();
    }
    for (int i = 0; i < 7; i++) {
        if (uint64_t next_move = board.get_move_mask(next_mask, i)) {
            BitBoard next_board(board);
            next_board.move(next_move);
            int score = -generate_book(depth-1, next_board, board_state += i+'1');
            board_state.pop_back();
            if (score > best_score) {
                best_move = i;
                best_score = score;
            }
        }
    }
    book[board.get_key()] = {board_state, best_move, best_score};
    return best_score;
}

int main() {
    std::ifstream input("../opening_book/book.txt");
    std::ofstream output("../opening_book/final_book.txt");
    if (!input.is_open()) {
        printf("Book file is not present in the same directory.\n Terminating...\n");
        return 1;
    }
    int score, move;
    uint64_t board_key; 
    std::string board_state;
    while (input >> board_key >> board_state >> move >> score) {
        book[board_key] = {board_state, move, score};
    }
    input.close();
    BitBoard board;
    generate_book(7, board, "");
    for (auto &[k,v] : book) {
        auto[a,b,c] = v;
        output << k << ' ' << a << ' ' << b << ' ' << c << '\n';
    }
    output.close();
}