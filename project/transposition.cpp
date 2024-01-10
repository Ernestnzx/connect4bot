#include "transposition.hpp"

Transposition::Transposition() {
    key_table = new key_t[TABLE_SIZE];
    value_table = new value_t[TABLE_SIZE];
}

Transposition::~Transposition() {
    delete key_table;
    delete value_table;
}

uint8_t Transposition::get_value(uint64_t board_key) {
    int idx = get_key(board_key);
    return (key_table[idx] == (key_t)board_key) ? value_table[idx] : 0;
}

void Transposition::place(uint8_t value, uint64_t board_key) {
    int idx = get_key(board_key);
    key_table[idx] = board_key;
    value_table[idx] = value;
}

void Transposition::reset() {
    memset(key_table, 0, sizeof(key_t)*TABLE_SIZE);
    memset(value_table, 0, sizeof(value_t)*TABLE_SIZE);
}

int Transposition::get_key(uint64_t board_key) {
    return board_key % TABLE_SIZE;
}

