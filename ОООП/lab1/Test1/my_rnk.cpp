#pragma once
#include "pch.h"
#include <cstddef>
#include "my_rnk.h"

    my_rnk::~my_rnk(){
        delete this->chain;
		chain = nullptr;
    }
    my_rnk::my_rnk(){
        lenth = 0;
        chain = new my_bit_chain;
    }
    my_rnk::my_rnk(my_bit_chain* new_chain){
        lenth = new_chain->size()/2;
        chain = new_chain;
    }
    my_rnk::my_rnk(nucleotide nuc, size_t amount){
        lenth = amount;
        chain = new my_bit_chain;
        chain->reserve(amount * 2);
		unsigned char nuc_byte = 0;
		nuc_byte = ((((((nuc_byte + (unsigned char)nuc) << 2) + (unsigned char)nuc) << 2) + (unsigned char)nuc) << 2) + (unsigned char)nuc;
			chain->put_bytes(amount/4, nuc_byte);
		amount = amount % 4;
        switch (nuc){
            case nucleotide::A:
                for (size_t i = 0; i<amount; i++){
                    chain->push_back(false);
                    chain->push_back(false);
                };
                break;
            case nucleotide::G:
                for (size_t i = 0; i<amount; i++){
                    chain->push_back(false);
                    chain->push_back(true);
                };
                break;
            case nucleotide::C:
                for (size_t i = 0; i<amount; i++){
                    chain->push_back(true);
                    chain->push_back(false);
                };
                break;
            case nucleotide::T:
                for (size_t i = 0; i<amount; i++){
                    chain->push_back(true);
                    chain->push_back(true);
                };
                break;
        }
    }
    my_rnk::my_rnk(const my_rnk & another_rnk){
        lenth = another_rnk.capacity();
        my_bit_chain *new_chain = new my_bit_chain();
		chain = new_chain;
        chain->reserve(lenth * 2);
		chain->copy(another_rnk.chain);
    }

    bool my_rnk::get_bit(size_t number) const{
        return (*chain)[number].get_bit();
    }
    size_t my_rnk::capacity() const{
        return lenth;
    }
    size_t my_rnk::cardinality(nucleotide nuc) const {
        size_t count = 0;
        char got_nuc = 0;
        for (unsigned int i = 0; i<lenth; i++){
            for (size_t j = 0; j<2; j++) {
                got_nuc = got_nuc << 1;
                got_nuc = got_nuc | (*chain)[i * 2 + j].get_bit();

            }
            if (got_nuc == static_cast<char>(nuc)){ count++; }
            got_nuc = 0;
        }
        return count;
    }

	std::unordered_map<nucleotide, int> my_rnk::cardinality() {
		std::unordered_map<nucleotide, int> my_map;
		for (size_t i = 0;i <lenth;i++) {
			my_map[(*this)[i]]++;
		}
		return my_map;
	}

    void my_rnk::trim(size_t last_index){
        if (last_index> lenth)return;
        chain->erase(last_index * 2);
        lenth = last_index ;
    }
    bool my_rnk::is_complimentary(my_rnk& another_rnk) const {
        if (lenth != another_rnk.capacity()){
            return false;
        }
        for (size_t i = 0; i<lenth * 2; i++){
            if ((*chain)[i].get_bit() == another_rnk.get_bit(i)){
                return false;
            }
        }
        return true;
    }
    my_rnk my_rnk::split(size_t index){
		if (index > lenth-1) {
			my_rnk bad_rnk;
			return bad_rnk;
		}
        my_bit_chain *new_chain = new my_bit_chain();
        new_chain->reserve((lenth - index) * 2);
        for (size_t i = index ; i<lenth; i++){
            new_chain->push_back((*chain)[i * 2].get_bit());
            new_chain->push_back((*chain)[i * 2 + 1].get_bit());
        }
        my_rnk new_rnk(new_chain);
        chain->erase(index * 2);
		lenth = index;
        return new_rnk;
    }
    my_rnk my_rnk::operator+(my_rnk& another_rnk){
        my_bit_chain *new_chain = new my_bit_chain;
        size_t new_lenth = another_rnk.capacity() + lenth;
        new_chain->reserve(new_lenth * 2);
		new_chain->unite(chain,another_rnk.chain);
        my_rnk new_rnk (new_chain);
		new_rnk.lenth =new_lenth;
        return new_rnk;
    }
    my_rnk my_rnk::operator+(nucleotide nuc){
        my_rnk new_rnk(nuc,1);
        return *this + new_rnk;
    }
    bool my_rnk::operator==(my_rnk& another_rnk){
        if (lenth != another_rnk.capacity()){
            return false;
        }
        for (size_t i = 0; i<lenth * 2; i++){
            if ((*chain)[i].get_bit() != another_rnk.get_bit(i)){
                return false;
            }
        }
        return true;
    }
    bool my_rnk::operator!=(my_rnk& another_rnk){
        return !(*this == another_rnk);
    }
    my_rnk my_rnk::operator!(){
        my_bit_chain *new_chain = new my_bit_chain;
        *new_chain = *chain;
        new_chain->flip();
        my_rnk new_rnk(new_chain);
        return new_rnk;
    }
	my_rnk& my_rnk::operator=(my_rnk another_rnk) {
		if (this == &another_rnk) {
			return *this;
		}
			my_bit_chain* new_chain = new my_bit_chain();
			new_chain->reserve(another_rnk.capacity() * 2);
			new_chain->copy(another_rnk.chain);
			lenth = another_rnk.lenth;
			delete chain;
			chain = new_chain;
		return *this;
	}

    nucleotide my_rnk::operator[] (size_t index) const{
        unsigned char nuc = 0;
        my_bit_chain const * buff_chain= chain;
        nuc = nuc + (*buff_chain)[index * 2];
        nuc = nuc << 1;
        nuc = nuc + (*buff_chain)[index * 2 + 1];
        switch (nuc)
        {
            case 0:return nucleotide::A;
            case 1:return nucleotide::G;
            case 2:return nucleotide::C;
            case 3:return nucleotide::T;
        }
    }
    my_bit_chain_buffer my_rnk::operator[](size_t index){
        return (*chain)[index*2];
    }
	my_rnk operator+(nucleotide nuc1, nucleotide nuc2) {
		my_rnk r1(nuc1, 1);
		my_rnk r2(nuc2, 1);
		return r1+r2;
	}
	my_rnk operator+(nucleotide nuc1, my_rnk r2) {
		my_rnk r1(nuc1, 1);
		return r1 + r2;
	}
	std::ostream& operator<<(std::ostream& os, my_rnk rnk)
	{
		for (size_t i = 0; i < rnk.capacity(); i++){
			os << rnk[i];
		}
		return os;
	}