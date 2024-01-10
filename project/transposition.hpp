#ifndef TRANSPOSITION_HPP
#define TRANSPOSITION_HPP

#include <cstdint>
#include <cstring>

class Transposition {
    private:
        typedef uint32_t key_t;
        typedef uint8_t value_t;
        const static int TABLE_SIZE = (1<<23)+9; 

        key_t *key_table;
        value_t *value_table;

        int get_key(uint64_t board_key);

    public:
        Transposition();
        ~Transposition();
        uint8_t get_value(uint64_t board_key);
        void place(uint8_t value, uint64_t board_key);
        void reset();
};

#endif
