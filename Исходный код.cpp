#include <iostream>
#include<vector>
using namespace std;
enum nucleotide{ A, G, C, T};

class nuc_buff{
	char* pos;
	char pos_in_byte;
};

class bit_chain{
	size_t lenth;
	char* bits;

	bit_chain(){
	
	}

};

class rnk{
	size_t lenth;
	vector<bool> *chain;
public:
	~rnk(){
		delete chain;
	}
	rnk(){
		lenth = 0;
		chain = new vector<bool>;
	}
	rnk(vector<bool>* new_chain){
		lenth = new_chain->size();
		chain = new_chain;
	}
	rnk(nucleotide nuc, size_t amount){
		lenth = amount;
		chain = new vector<bool>;
		chain->reserve(amount * 2);
		switch (nuc){
		case A:
			for (int i = 0; i<amount; i++){
				chain->push_back(false);
				chain->push_back(false);
			};
		case G:
			for (int i = 0; i<amount; i++){
				chain->push_back(false);
				chain->push_back(true);
			};
		case C:
			for (int i = 0; i<amount; i++){
				chain->push_back(true);
				chain->push_back(false);
			};
		case T:
			for (int i = 0; i<amount; i++){
				chain->push_back(true);
				chain->push_back(true);
			};
		}
	}
	rnk(rnk const & another_rnk){
		lenth = another_rnk.get_lenth;
		vector<bool> *chain = new vector<bool>;
		chain->reserve(lenth * 2);
		for (size_t i = 0; i < lenth * 2; i++){
			(*chain)[i] = (*another_rnk.chain)[i];
		}
	}

	bool get_bit(size_t number){
		return (*chain)[number];
	}
	size_t get_lenth(){
		return lenth;
	}
	size_t capacity(){
		return lenth;
	}
	size_t cardinality(nucleotide nuc){
		size_t count = 0;
		char got_nuc = 0;
		for (unsigned int i = 0; i<lenth; i++){
			for (int j = 0; j<2; j++) {
				got_nuc = got_nuc << 1;
				got_nuc = got_nuc | (*chain)[i * 2 + j];

			}
			if (got_nuc == nuc){ count++; }
			got_nuc = 0;
		}
		return count;
	}
	void trim(size_t last_index){
		if (last_index> lenth)return;
		chain->erase(chain->begin() + last_index * 2, chain->end());
		lenth = last_index + 1;
	}
	bool isComplimentary(rnk& another_rnk){
		if (lenth != another_rnk.get_lenth()){
			return false;
		}
		for (size_t i = 0; i<lenth * 2; i++){
			if ((*chain)[i] == another_rnk.get_bit(i)){
				return false;
			}
		}
		return true;
	}

	rnk operator+(rnk& another_rnk){
		vector<bool> *new_chain = new vector<bool>;
		int new_lenth = another_rnk.get_lenth() + lenth;
		new_chain->reserve(new_lenth * 2);
		size_t i = 0;
		for (i; i<lenth * 2; i++){
			(*new_chain)[i] = (*chain)[i];
		}
		for (i; i<new_lenth * 2; i++){
			(*new_chain)[i] = another_rnk.get_bit(i - lenth * 2);
		}
		rnk *new_rnk = new rnk(new_chain);
		return *new_rnk;
	}
	bool operator==(rnk& another_rnk){
		if (lenth != another_rnk.get_lenth()){
			return false;
		}
		for (size_t i = 0; i<lenth * 2; i++){
			if ((*chain)[i] != another_rnk.get_bit(i)){
				return false;
			}
		}
		return true;
	}
	bool operator!=(rnk& another_rnk){
		return !(*this == another_rnk);
	}
	rnk operator!(){
		vector<bool> *new_chain = new vector<bool>;
		*new_chain = *chain;
		new_chain->flip;
		rnk *new_rnk = new rnk(new_chain);
		return *new_rnk;
	}
	nuc_buff operator[](size_t number){

	}

};



int main() {
	cout << "Hello, World!" << endl;
	return 0;
}