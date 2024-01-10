#include "transposition.hpp"

Transposition::Transposition() {
    table.assign(TABLE_SIZE, 0);
}

uint8_t Transposition::get_value(uint64_t hashkey) {
    size_t key = get_key(hashkey);
    return is_present(key, hashkey) ? table[key] & VAL_MASK : 0;
}

void Transposition::place(uint8_t value, uint64_t hashkey) {
    table[get_key(hashkey)] = (hashkey << KEY_SHIFT) | value;
}

size_t Transposition::get_key(uint64_t hashkey) {
    return hashkey % TABLE_SIZE;
}

bool Transposition::is_present(size_t key, uint64_t hashkey) {
    return table[key] >> KEY_SHIFT == hashkey;
}
