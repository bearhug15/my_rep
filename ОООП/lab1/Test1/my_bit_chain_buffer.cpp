#pragma once
#include "pch.h"
#include "my_bit_chain_buffer.h"

    my_bit_chain_buffer::my_bit_chain_buffer(unsigned char* bit, int pos) : bit(bit), pos(pos){};
    my_bit_chain_buffer::~my_bit_chain_buffer(){};
    my_bit_chain_buffer& my_bit_chain_buffer::operator= (my_bit_chain_buffer other_buffer){
        bool other_bit = other_buffer.get_bit();
        if (other_bit == true){
            (*bit) = (*bit) | (((unsigned char)1) << (7 - pos ));
        }
        else{
            unsigned char buff = 255;
			(*bit) = (*bit) & (buff ^ (((unsigned char)1) << (7 - pos)));
        }
		other_bit = other_buffer.get_next_bit();
		if (other_bit == true) {
			(*bit) = (*bit) | (((unsigned char)1) << (6 - pos));
		}
		else {
			unsigned char buff = 255;
			(*bit) = (*bit) & (buff ^ (((unsigned char)1) << (6 - pos)));
		}
		return *this;
    }
	my_bit_chain_buffer& my_bit_chain_buffer::operator= (nucleotide nuc) {
		unsigned char cnuc = static_cast<unsigned char>(nuc);
		(*bit) = ((*bit) & (255 ^ (3 << (6 - pos))));
		(*bit) = (*bit) | (cnuc << (6 - pos));
		return *this;
	}
    my_bit_chain_buffer& my_bit_chain_buffer::operator= (bool other_bit){
        if (other_bit == true){
            (*bit) = *(bit) | (((unsigned char)1) << (7 - pos));
        }
        else{
            unsigned char buff = 255;
            (*bit) = (*bit)& (buff ^ (((unsigned char)1) << (7 - pos)));
        }
        return *this;
    }

    bool my_bit_chain_buffer::get_bit()const {
		unsigned char buff = (*bit);
		buff = buff << pos;
		buff = buff >> 7;
        return (bool)buff;
    }
	bool my_bit_chain_buffer::get_next_bit()const {
		unsigned char buff = (*bit);
		buff = buff << (pos+1);
		buff = buff >> 7;
		return (bool)buff;
	}

	my_bit_chain_buffer::operator nucleotide() {
		unsigned char nuc = 0;
		nuc = (*bit);
		nuc= nuc<< pos;
		nuc = nuc >> 7;
		nuc = nuc << 1;
		unsigned char buff_nuc = (*bit) << (pos + 1);
		buff_nuc = buff_nuc >> 7;
		nuc = nuc | buff_nuc;
		switch (nuc)
		{
		case static_cast<int>(nucleotide::A) : return nucleotide::A;
		case static_cast<int>(nucleotide::G) : return nucleotide::G;
		case static_cast<int>(nucleotide::C) : return nucleotide::C;
		case static_cast<int>(nucleotide::T) : return nucleotide::T;
		}
	};
	std::ostream& operator<<(std::ostream& os, my_bit_chain_buffer buff) {
		unsigned char nuc = 0;
		nuc = (unsigned char)buff.get_bit();
		nuc=nuc << 1;
		unsigned char buff_nuc = (unsigned char)buff.get_next_bit();
		nuc = nuc | buff_nuc;
		switch (nuc)
		{
		case 0: os << "A"; break;
		case 1: os << "G"; break;
		case 2: os << "C"; break;
		case 3: os << "T"; break;
		default: os.setstate(std::ios_base::failbit);
		}
		return os;
	}