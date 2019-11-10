#pragma once
#include "pch.h"
#ifndef LAB_DNK_BIT_CHAIN_BUFFER_H
#define LAB_DNK_BIT_CHAIN_BUFFER_H
#include "nucleotide.h"
//enum nucleotide{A, G, C, T};
class my_bit_chain_buffer{
    unsigned char* bit;
    int pos;
public:
    my_bit_chain_buffer(unsigned char* bit, int pos);
    ~my_bit_chain_buffer();
    my_bit_chain_buffer& operator= (my_bit_chain_buffer other_buffer);
    my_bit_chain_buffer& operator= (bool other_bit);
	my_bit_chain_buffer& operator= (nucleotide nuc);
    bool get_bit()const;
	bool get_next_bit()const;
	operator nucleotide();
};
std::ostream& operator<<(std::ostream& os, my_bit_chain_buffer buff);
#endif //LAB_DNK_BIT_CHAIN_BUFFER_H
