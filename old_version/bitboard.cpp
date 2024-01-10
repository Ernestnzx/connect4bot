#include "bitboard.hpp"

BitBoard::BitBoard() {
    num_moves = 0;
    masks[0] = masks[1] = 0;
    memset(moves, 0, sizeof(moves));
    // assigning the DAT table for shifts
    for (int i = 0; i < 7; i++) {
        next_placement[i] = i*7;
    }
}

BitBoard::BitBoard(std::string board_state, bool &is_p1_turn) : BitBoard() {
    for (char &c : board_state) {
        int col = c - '1';
        assert(0 <= col && col <= 6 && is_move_valid(col));
        placed_tile(col, is_p1_turn); 
        is_p1_turn = !is_p1_turn;
    }
}

void BitBoard::debug_info() {
    printf("DEBUG INFORMATION\n");
    printf("=================\n");
    printf("Board id: %s\n", board_id.c_str());
    printf("mask for p1: 0x%011lx\n", masks[1]);
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {
            printf("%lu", masks[1] >> (j*7+i) & 1);
        }
        printf("\n");
    }
    printf("mask for p2: 0x%011lx\n", masks[0]);
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {
            printf("%lu", masks[0] >> (j*7+i) & 1);
        }
        printf("\n");
    }
    printf("\n");
    printf("DAT (next_placement):\n");
    for (auto &i : next_placement)
        printf("%2d ", i);
    printf("\n");
}

bool BitBoard::is_board_valid() {
    return (masks[0] & masks[1]) == 0;
}

void BitBoard::print_board() {
    printf("[2J[;H"); // this is to clear screen
    printf("Welcome to a game of CONNECT 4\n");
    printf("==============================\n");
    for (int i = 5; i >= 0; i--) {
        for (int j = 0; j < 7; j++) {
            if (masks[1] >> (j*7+i) & 1) {
                printf("%c ", PLAYER_ONE_TILE);
            } else if (masks[0] >> (j*7+i) & 1) {
                printf("%c ", PLAYER_TWO_TILE);
            } else  {
                printf("%c ", BLANK);
            }
        }
        printf("\n");
    }
    debug_info();
}

void BitBoard::print_outcome() {
    print_board();
    if (is_board_full()) {
        printf("Draw! The board is full.\n");
    } else if (is_player_won(true)) {
        printf("Player one has won!\n");
    } else {
        printf("Player two has won!\n");
    }
}

void BitBoard::placed_tile(int col, bool is_p1_turn) {
    masks[is_p1_turn] ^= 1lu << next_placement[col];
    next_placement[col]++;
    moves[num_moves++] = col;
    // assert(is_board_valid());
}

void BitBoard::undo_tile(bool is_p1_turn) {
    int col = moves[--num_moves];
    next_placement[col]--;
    masks[is_p1_turn] ^= 1lu << next_placement[col];
    // assert(is_board_valid());
}

bool BitBoard::is_game_over() {
    return is_board_full() || is_player_won(true) || is_player_won(false);
}

bool BitBoard::is_board_full() {
    return num_moves == NROWS*NCOLS;
}

bool BitBoard::is_player_won(bool is_player_one) {
    uint64_t &mask = masks[is_player_one];
    for (int dir : directions) {
        if (mask & (mask >> dir) & (mask >> 2*dir) & (mask >> 3*dir)) {
            return true;
        }
    }
    return false;
}

bool BitBoard::is_move_valid(int col) {
    return (0 <= col && col <= 6) && next_placement[col] < 6 + 7*col;
}

int BitBoard::get_score() {
    return (NROWS*NCOLS+2 - num_moves)/2;
}

int BitBoard::get_upper_bound() {
    return (NROWS*NCOLS-1 - num_moves)/2;
}

uint64_t BitBoard::get_hashkey(bool is_p1_turn) {
    return (masks[0] | masks[1]) + masks[is_p1_turn];
}

std::string BitBoard::get_board_string() {
    return board_id;
}

std::pair<uint64_t,uint64_t> BitBoard::get_mask() {
    return std::pair<uint64_t,uint64_t>(masks[1], masks[0]);
}
