#ifndef TRANSPOSITION_HPP
#define TRANSPOSITION_HPP

#include <cstdint>
#include <vector>

class Transposition {
    private:
        const static int KEY_SHIFT = 8;
        const static uint64_t VAL_MASK = 0xff;
        const static size_t TABLE_SIZE = 8388593; 
        // prime number slightly under 64MB using uint64_t

        std::vector<uint64_t> table;

        size_t get_key(uint64_t hashkey);
        bool is_present(size_t key, uint64_t hashkey);

    public:
        Transposition();
        uint8_t get_value(uint64_t hashkey);
        void place(uint8_t value, uint64_t hashkey);
};

#endif
