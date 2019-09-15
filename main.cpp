#include <iostream>
#include<vector>
using namespace std;
enum nucleotide{ A, G, C, T};
//char or_mask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
class nuc_buff{
    char* pos;
    char pos_in_byte;
};

class bit_chain_buffer{
    char* bit;
    int pos;
public:
    bit_chain_buffer(char* bit,int pos): bit(bit),pos(pos){};
   ~bit_chain_buffer(){};
    bit_chain_buffer operator= (bit_chain_buffer other_buffer){
        char changed_bit = *bit;
        bool other_bit = other_buffer.get_bit();
        if(other_bit==true){
            (*bit)= *(bit)|(((char)other_bit)<<(8-pos-1));
        }else{
            char buff=0xff;
            (*bit)= *(bit) & (buff^(((char)other_bit)<<(8-pos-1)));
        }
        delete &other_buffer;
        return *this;
    }

    bit_chain_buffer operator= (bool other_bit){
        if(other_bit==true){
            (*bit)= *(bit)|(((char)other_bit)<<(8-pos-1));
        }else{
            char buff=0xff;
            (*bit)= *(bit) & (buff^(((char)other_bit)<<(8-pos-1)));
        }
        return *this;
    }

    bool get_bit(){
        return ((*bit)<<(pos)>>7);
    }
};

class bit_chain{
    size_t used_lenth;
    size_t full_lenth;
    char* bits;
public:
    bit_chain(){
        used_lenth=0;
        full_lenth=16;
        char* bits= new char[2];
    }
    ~bit_chain(){
        delete[] bits;
    }
    void reserve(size_t new_size){
        delete [] bits;
        char *bits = new char[new_size/8+2];
    }
    size_t size(){
        return used_lenth;
    }
    size_t max_size(){
        return full_lenth;
    }
    void push_back(bool bit){
        if(used_lenth== full_lenth) {
            char *new_bits = new char[full_lenth / 8 * 1.5];
            for (size_t i = 0; i < full_lenth / 8; i++) {
                new_bits[i] = bits[i];
            }
            full_lenth = (full_lenth / 8 * 1.5) * 8;
            delete[] bits;
            bits = new_bits;
        }
        bits[used_lenth/8]= bits[used_lenth/8] & ((0+bit)<<(used_lenth%8));
        used_lenth++;
    }
    bit_chain_buffer operator[](size_t index){
        bit_chain_buffer *bit_buffer = new bit_chain_buffer((bits+index/8),index%8);
        return *bit_buffer;
    }
    void flip(){
        char buff=0xff;
        for(size_t i = 0; i<full_lenth/8;i++){
            bits[i]= bits[i]^buff;
        }
    }
    void erase(size_t start){
        if (start >=used_lenth){
            return;
        }
        else{
            char * new_bits = new char[start*2/8+1];
            full_lenth = (start*2/8+1)*8;
            for(size_t i=0;i<start*2/8+1;i++){
                new_bits[i]=bits[i];
            }
            delete [] bits;
            bits= new_bits;
            used_lenth = start;
        }
    }
};


class rnk{
    size_t lenth;
    bit_chain *chain;
public:
    ~rnk(){
        delete[] chain;
    }
    rnk(){
        lenth = 0;
        chain = new bit_chain;
    }
    rnk(bit_chain* new_chain){
        lenth = new_chain->size();
        chain = new_chain;
    }
    rnk(nucleotide nuc, size_t amount){
        lenth = amount;
        chain = new bit_chain;
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
        lenth = another_rnk.get_lenth();
        bit_chain *chain = new bit_chain;
        chain->reserve(lenth * 2);
        for (size_t i = 0; i < lenth * 2; i++){
            (*chain)[i] = (*another_rnk.chain)[i];
        }
    }

    bool get_bit(size_t number) const{
        return (*chain)[number].get_bit();
    }

    size_t get_lenth() const{
        return lenth;
    }
    size_t capacity() const {
        return lenth;
    }
    size_t cardinality(nucleotide nuc) const {
        size_t count = 0;
        char got_nuc = 0;
        for (unsigned int i = 0; i<lenth; i++){
            for (int j = 0; j<2; j++) {
                got_nuc = got_nuc << 1;
                got_nuc = got_nuc | (*chain)[i * 2 + j].get_bit();

            }
            if (got_nuc == nuc){ count++; }
            got_nuc = 0;
        }
        return count;
    }
    void trim(size_t last_index){
        if (last_index> lenth)return;
        chain->erase(last_index*2);
        lenth = last_index + 1;
    }
    bool is_complimentary(rnk& another_rnk) const {
        if (lenth != another_rnk.get_lenth()){
            return false;
        }
        for (size_t i = 0; i<lenth * 2; i++){
            if ((*chain)[i].get_bit() == another_rnk.get_bit(i)){
                return false;
            }
        }
        return true;
    }
    rnk split(size_t index){
        bit_chain *new_chain = new bit_chain();
        new_chain->reserve((lenth-index+1)*2);
        for(size_t i=index*2;i<lenth*2;i++){
            new_chain->push_back((*chain)[i*2].get_bit());
            new_chain->push_back((*chain)[i*2+1].get_bit());
        }
        rnk *new_rnk= new rnk(new_chain);
        chain->erase(index*2);
        return *new_rnk;
    }

    rnk operator+(rnk& another_rnk){
        bit_chain *new_chain = new bit_chain;
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
            if ((*chain)[i].get_bit() != another_rnk.get_bit(i)){
                return false;
            }
        }
        return true;
    }
    bool operator!=(rnk& another_rnk){
        return !(*this == another_rnk);
    }
    rnk operator!(){
        bit_chain *new_chain = new bit_chain;
        *new_chain = *chain;
        new_chain->flip();
        rnk *new_rnk = new rnk(new_chain);
        return *new_rnk;
    }
    nuc_buff operator[](size_t number){

    }

};

class dnk{
    rnk* first_rnk;
    rnk* second_rnk;
public:
    explicit dnk(rnk& first_rnk, rnk& second_rnk){
        if(first_rnk.is_complimentary(second_rnk)){
            this->first_rnk=&first_rnk;
            this->second_rnk=&second_rnk;
        }
        else{
            cout<<"Rnk isn`t complimentary";
        }
    }
    rnk& get_first_rnk() const {
        return *first_rnk;
    }
    rnk& get_second_rnk() const {
        return *second_rnk;
    }
};



int main() {
    cout << "Hello, World!" << endl;
    return 0;
}