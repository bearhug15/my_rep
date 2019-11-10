#pragma once
#include "pch.h"
#include "my_bit_chain.h"

    my_bit_chain::my_bit_chain(){
        used_lenth = 0;
        full_lenth = 0;
        //unsigned char* bits = new unsigned char[2];
		bits = nullptr;
    }
    my_bit_chain::~my_bit_chain(){
        delete[] bits;
		bits = nullptr;
    }
    void my_bit_chain::reserve(size_t new_size){
        delete[] this->bits;
        unsigned char *new_bits = new unsigned char[new_size / 8 + 2];
		bits = new_bits;
        used_lenth=0;
        full_lenth=(new_size / 8 + 2)*8;
    }
    size_t my_bit_chain::size(){
        return used_lenth;
    }
    size_t my_bit_chain::max_size(){
        return full_lenth;
    }
	void my_bit_chain::push_nuc(nucleotide nuc) {
		if (used_lenth == full_lenth) {
			unsigned char* new_bits = new unsigned char[size_t(full_lenth / 8 * 1.5+1)];
			for (size_t i = 0; i < full_lenth / 8; i++) {
				new_bits[i] = bits[i];
			}
			full_lenth =size_t(full_lenth / 8 * 1.5+1) * 8;
			delete[] bits;
			bits = new_bits;
		}
		unsigned char bit_nuc = static_cast<unsigned char>(nuc);
		bit_nuc = bit_nuc << (6 - used_lenth % 8);
		unsigned char mask = 3;
		mask =((unsigned char)255) ^ (mask << (6 - used_lenth % 8));
		bits[(used_lenth) / 8] = (bits[(used_lenth) / 8] &mask)| bit_nuc ;
		used_lenth += 2;
	}

    my_bit_chain_buffer my_bit_chain::operator[](size_t index){
        my_bit_chain_buffer bit_buffer((bits + index / 8), index % 8);
        return bit_buffer;
    }
    const bool my_bit_chain::operator[](size_t index) const {
		unsigned char buff = bits[index / 8];
		buff = buff << (index % 8);
		buff = buff >> 7;
        if (buff == 1){
            return true;
        }
        else{
            return false;
        }
    }
    void my_bit_chain::flip(){
        unsigned char buff = 255;
        for (size_t i = 0; i<full_lenth / 8; i++){
            bits[i] = bits[i] ^ buff;
        }
    }
    void my_bit_chain::erase(size_t start){
        if (start > full_lenth){
            return;
        }
        else{
           unsigned char * new_bits = new unsigned char[start / 8 + 1];
            full_lenth = (start / 8 + 1) * 8;
            for (size_t i = 0; i<start / 8 + 1; i++){
                new_bits[i] = bits[i];
            }
            delete[] bits;
            bits = new_bits;
            used_lenth = start;
        }
    }
	void my_bit_chain::copy(my_bit_chain* another_chain) {
		for (size_t i = 0; i < (another_chain->max_size() / 8); ++i) {
			(this->bits)[i] = (another_chain->bits)[i]; 
		}
		used_lenth = another_chain->used_lenth;
	}
	void my_bit_chain::unite(my_bit_chain* first_chain, my_bit_chain* second_chain) {
		
		size_t i = 0;
		used_lenth = first_chain->used_lenth;
		for (; i < used_lenth; i++) {
			(*this)[i] = (*first_chain)[i];
		}
		for (; i < used_lenth+ second_chain->used_lenth * 2; i++) {
			(*this)[i] = (*second_chain)[i-used_lenth];
		}
		used_lenth += second_chain->used_lenth;

	}
	void my_bit_chain::operator=(my_bit_chain &another_chain) {
		used_lenth = another_chain.used_lenth;
		delete[] bits;
		bits = new unsigned char [another_chain.full_lenth/8];
		for (size_t i = 0; i < (another_chain.full_lenth / 8); ++i) {
			bits[i] = (another_chain.bits)[i];
		}
		full_lenth = another_chain.full_lenth;
	}
	void my_bit_chain::put_bytes(size_t last_pos, unsigned char byte){
		for (size_t i = 0; i < last_pos; i++){
			bits[i] = byte;
		}
		used_lenth = last_pos * 8;
	}