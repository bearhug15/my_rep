#pragma once
#include "my_bit_chain_buffer.h"
#ifndef LAB_DNK_BIT_CHAIN_H
#define LAB_DNK_BIT_CHAIN_H
class my_bit_chain{
    size_t used_lenth;
    size_t full_lenth;
    unsigned char* bits;
public:
    my_bit_chain();
    ~my_bit_chain();
    void reserve(size_t new_size);
    size_t size();
    size_t max_size();
    void push_back(bool bit);
    my_bit_chain_buffer operator[](size_t index);
    const bool operator[](size_t index) const ;
	void put_bytes(size_t last_pos, char byte);
    void flip();
    void erase(size_t start);
	void copy(my_bit_chain* another_chain);
	void unite(my_bit_chain* first_chain, my_bit_chain* second_chain);
	void operator=(my_bit_chain &another_chain);
};

#endif //LAB_DNK_BIT_CHAIN_H
