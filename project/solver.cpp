#include "solver.hpp"

Solver::Solver() {
    explored = 0;
}

int Solver::solve(BitBoard &board) {
    if (board.can_win_next()) return board.get_score();
    int min_val = board.get_losing_score();
    int max_val = board.get_score();
    while (min_val < max_val) {
        int mid = (max_val + min_val-1)/2;
        if (mid <= 0 && min_val/2 <= mid) mid = min_val / 2;
        if (mid >= 0 && max_val/2 >= mid) mid = max_val / 2;
        int eval = negamax(board, mid, mid+1);
        (eval <= mid) ? max_val = eval : min_val = eval;
    }
    return min_val;
}

int Solver::negamax(BitBoard &board, int alpha, int beta) {
    explored++;
    uint64_t next_moves = board.get_non_losing_moves();
    if (next_moves == 0) {
        return board.get_losing_score();
    }
    if (board.is_game_drawn()) {
        return 0;
    }

    // tightning the bounds on the search window 
    // and possibly returning early
    int min_val = board.get_lower_bound();
    int max_val = board.get_upper_bound();
    if (alpha < min_val) {
        alpha = min_val;
        if (alpha >= beta) return alpha;
    }
    if (beta > max_val) {
        beta = max_val;
        if (alpha >= beta) return beta;
    }
    uint64_t board_key = board.get_key();
    if (int val = table.get_value(board_key)) {
        if (val > board.MAX_SCORE - board.MIN_SCORE) {
            min_val = val - board.MAX_SCORE + 2*board.MIN_SCORE;
            if (alpha < min_val) {
                alpha = min_val;
                if (alpha >= beta) return alpha;
            }
        } else {
            max_val = val + board.MIN_SCORE;
            if (beta > max_val) {
                beta = max_val;
                if (alpha >= beta) return beta;
            }
        }
    }

    // order the moves that gives the largest possible winning chance
    MoveSorter move_sorter; 
    for (int move : move_order)
        if (uint64_t next_move = board.get_move_mask(next_moves, move))
            move_sorter.add(next_move, board.get_move_score(next_move));

    // search the moves that give the largest chance of winning
    while (uint64_t next_move = move_sorter.get()) {
        BitBoard next_pos(board);
        next_pos.move(next_move);
        int val = -negamax(next_pos, -beta, -alpha);
        if (val >= beta) {
            table.place(val + board.MAX_SCORE - 2*board.MIN_SCORE, board_key);
            return val;
        }
        alpha = std::max(alpha, val);
    }
    table.place(alpha - board.MIN_SCORE, board_key);
    return alpha;
}

void Solver::reset() {
    table.reset();
    explored = 0;
}

unsigned long long Solver::num_nodes() {
    return explored;
}
