#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>

class BitBoard {
    private:
        static const int NROWS = 6;
        static const int NCOLS = 7;
        static const char BLANK = '.';
        static const char PLAYER_ONE = 'X';
        static const char PLAYER_TWO = 'O';
        static const char MASK_CHECK = '#';
        static const uint64_t FULL_BOTTOM_MASK = 0x40810204081;
        static const uint64_t FULL_BOARD_MASK = 0xFDFBF7EFDFBF;

        int direction[3] = {6,7,8};
        uint64_t curr_pos;
        uint64_t mask; 
        uint64_t moves;

        void print_mask(uint64_t mask);
        uint64_t top_mask(int col);
        uint64_t column_mask(int col);
        uint64_t bottom_mask(int col);
        uint64_t possible_mask();
        uint64_t winning_moves();
        uint64_t opponent_winning_moves();
        uint64_t compute_winning_moves(uint64_t pos, uint64_t mask);

    public:
        static const int MIN_SCORE = -(NROWS*NCOLS)/2+3;
        static const int MAX_SCORE = (NROWS*NCOLS+1)/2-3;

        BitBoard();
        int get_score();
        int get_losing_score();
        int get_lower_bound();
        int get_upper_bound();
        int get_move_score(uint64_t move);
        bool can_win_next();
        bool is_valid(int col);
        bool is_winning_move(int col);
        bool is_game_drawn();
        void debug();
        void move(uint64_t mask);
        void move(std::string board_state);
        void reset();
        uint64_t get_key();
        uint64_t get_move_mask(uint64_t next_moves, int col);
        uint64_t get_non_losing_moves();
};

#endif
