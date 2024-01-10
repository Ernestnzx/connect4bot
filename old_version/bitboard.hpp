#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <random>
#include <string>
#include <utility>
#include <vector>

class BitBoard {
    private:
        // constants
        static const int NROWS = 6;
        static const int NCOLS = 7;
        static const char BLANK = '.';
        static const char PLAYER_ONE_TILE = 'X';
        static const char PLAYER_TWO_TILE = 'O';
        static const uint64_t FULL_MASK = 0xFDFBF7EFDFBF;

        // attributes of the board
        int num_moves;
        uint64_t masks[2];
        int next_placement[7];
        int directions[4] = {1,7,6,8};
        int moves[42];
        std::string board_id;

        void debug_info();
        bool is_board_valid();

    public:
        static const int MIN_SCORE = -(NROWS*NCOLS)/2+3;
        static const int MAX_SCORE = (NROWS*NCOLS+1)/2-3;
 
        BitBoard();
        BitBoard(std::string board_state, bool &is_p1_turn);
        void placed_tile(int col, bool is_p1_turn);
        void print_board();
        void print_outcome();
        void undo_tile(bool is_p1_turn);
        bool is_board_full();
        bool is_game_over();
        bool is_player_won(bool is_player_one);
        bool is_move_valid(int col);
        int get_score();
        int get_upper_bound();
        uint64_t get_hashkey(bool is_p1_turn);
        std::string get_board_string();
        std::pair<uint64_t,uint64_t> get_mask();
};

#endif
