#pragma once
#include "pch.h"
#include "my_bit_chain.h"
#include<unordered_map>
#ifndef LAB_DNK_RNK_H
#define LAB_DNK_RNK_H


/*class rnk {

};*/
class my_rnk{
    size_t lenth;
    my_bit_chain *chain;
public:
    ~my_rnk();
    my_rnk();
    my_rnk(my_bit_chain* new_chain);
    my_rnk(nucleotide nuc, size_t amount);
    my_rnk(const my_rnk & another_rnk);

    bool get_bit(size_t number) const;
    size_t capacity() const;
    size_t cardinality(nucleotide nuc) const;
	std::unordered_map<nucleotide, int> cardinality();
    void trim(size_t last_index);
    bool is_complimentary(my_rnk& another_rnk) const;
    my_rnk split(size_t index);
    my_rnk operator+(my_rnk& another_rnk);
    my_rnk operator+(nucleotide nuc);
    bool operator==(my_rnk& another_rnk);
    bool operator!=(my_rnk& another_rnk);
	my_rnk operator!();
	my_rnk& operator=(my_rnk another_rnk);

    nucleotide operator[](size_t index) const;
    my_bit_chain_buffer operator[](size_t index);
	
};
my_rnk operator+(nucleotide, nucleotide);
my_rnk operator+(nucleotide, my_rnk);
std::ostream& operator<<(std::ostream& os, my_rnk rnk);
#endif //LAB_DNK_RNK_H
