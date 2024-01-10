#include "bitboard.hpp"
#include "solver.hpp"
#include <chrono>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream in_file("../test_case/begin_medium.txt", std::ios::in);
    if (!in_file.is_open()) {
        printf("File name does not exist. Terminating program...\n");
        return 1;
    }
    std::string line;
    int tc = 0, ac = 0;
    unsigned long long total_node = 0, total_time = 0;
    printf("Reading testcases...\n");
    printf("============================\n");
    Solver solver;
    BitBoard board;
    while (getline(in_file, line)) {
        std::stringstream ss(line);
        std::string state; int score;
        ss >> state >> score;
        solver.reset();
        board.reset();
        board.move(state);
        auto start = std::chrono::high_resolution_clock::now();
        int negamax_score = solver.solve(board);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        total_time += duration.count();
        total_node += solver.num_nodes();
        if (negamax_score == score) {
            printf("Test Case: #%3d Accepted\n", tc++); 
            ac++;
        } else {
            printf("Test Case: #%3d Wrong Answer -> expected %d, got %d\n", 
                tc++, score, negamax_score);
        }
        printf("Nodes explored: %llu\n", solver.num_nodes());
        printf("Time elapsed: %ld ms\n", duration.count());
        printf("===========================\n");
    }
    printf("Score: %d/%d\n", ac, tc);
    printf("Average time: %.3lf ms\n", (double)total_time / tc);
    printf("Average number of positions explored: %llu nodes\n", total_node / tc);
    printf("Number of positions per second: %.3llu\n", 1000*total_node / total_time);
    in_file.close();
}
