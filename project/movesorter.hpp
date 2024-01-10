#ifndef MOVESORTER_HPP
#define MOVESORTER_HPP

#include <cstdint>
#include <utility>

class MoveSorter {
    private:
        int num_moves;
        std::pair<uint64_t, int> sorted_moves[7];

    public:
        MoveSorter();
        void add(uint64_t moves, int score);
        uint64_t get();
};

#endif
