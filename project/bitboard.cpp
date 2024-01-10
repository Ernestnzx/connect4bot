#include "bitboard.hpp"

BitBoard::BitBoard() {
    mask = moves = curr_pos = 0;
}

int BitBoard::get_score() {
    return (NROWS*NCOLS - moves)/2;
}

int BitBoard::get_losing_score() {
    return -(NROWS*NCOLS-1 - moves)/2;
}

int BitBoard::get_lower_bound() {
    return -(NROWS*NCOLS-2 - moves)/2;
}

int BitBoard::get_upper_bound() {
    return (NROWS*NCOLS-1 - moves)/2;
}

int BitBoard::get_move_score(uint64_t move) {
    return __builtin_popcount(compute_winning_moves(curr_pos | move, mask));
}

bool BitBoard::can_win_next() {
    return winning_moves() & possible_mask();
}

bool BitBoard::is_valid(int col) {
    return !(mask & top_mask(col));
}

bool BitBoard::is_winning_move(int col) {
    return winning_moves() & possible_mask() & column_mask(col);
}

bool BitBoard::is_game_drawn() {
    return moves >= NROWS*NCOLS-2;
}

void BitBoard::move(uint64_t play) {
    curr_pos ^= mask;
    mask |= play;
    moves++;
}

void BitBoard::move(std::string board_state) {
    for (char c : board_state) {
        int col = c - '1';
        assert(0 <= col && col < 7 && 
            is_valid(col) && !is_winning_move(col));
        move((mask + bottom_mask(col)) & column_mask(col));
    }
}

void BitBoard::reset() {
    curr_pos = mask = moves = 0;
}

void BitBoard::debug() {
    printf("CURR_POSITION test\n");
    printf("==================\n");
    print_mask(curr_pos);

    printf("MASK test\n");
    printf("==================\n");
    print_mask(mask);

    printf("POSSIBLE_MASK test\n");
    printf("==================\n");
    print_mask(possible_mask());

    printf("WINNING_MASK test\n");
    printf("=================\n");
    print_mask(winning_moves());
}

uint64_t BitBoard::get_key() {
    return curr_pos + mask;
}

uint64_t BitBoard::get_move_mask(uint64_t next_moves, int col) {
    return next_moves & column_mask(col);
}

void BitBoard::print_mask(uint64_t mask) {
    for (int i = 6; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {
            printf("%c ", mask >> (j*(NROWS+1)+i) & 1 
                ? MASK_CHECK : BLANK);
        }
        printf("\n");
    }
    printf("\n");
}

uint64_t BitBoard::get_non_losing_moves() {
    uint64_t possible = possible_mask();
    uint64_t opponent_win = opponent_winning_moves();
    uint64_t forced = possible & opponent_win;
    if (forced & (forced - 1)) {
        return 0;
    }
    // removing possible moves under a opponent's winning position
    return (forced ? forced : possible) & ~(opponent_win >> 1); 
}

uint64_t BitBoard::top_mask(int col) {
    return 1ull << (col*(NROWS+1) + NROWS);
}

uint64_t BitBoard::bottom_mask(int col) {
    return 1ull << col*(NROWS+1);
}

uint64_t BitBoard::column_mask(int col) {
    return ((1ull << NROWS)-1) << col*(NROWS+1);
}

uint64_t BitBoard::possible_mask() {
    return (mask + FULL_BOTTOM_MASK) & FULL_BOARD_MASK;
}

uint64_t BitBoard::winning_moves() {
    return compute_winning_moves(curr_pos, mask);
}

uint64_t BitBoard::opponent_winning_moves() {
    return compute_winning_moves(curr_pos ^ mask, mask);
}

uint64_t BitBoard::compute_winning_moves(uint64_t pos, uint64_t mask) {
    // checking for 3 in a row, with one tile missing
    uint64_t r = (pos << 1) & (pos << 2) & (pos << 3), t;
    for (int dir : direction) {
        t = (pos << dir) & (pos << 2*dir);
        r |= t & (pos << 3*dir);
        r |= t & (pos >> dir);
        t >>= 3*dir;
        r |= t & (pos << dir);
        r |= t & (pos >> 3*dir);
    }
    return r & (FULL_BOARD_MASK ^ mask);
}
