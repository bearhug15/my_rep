#pragma once
#include "pch.h"
#include "my_dnk.h"
	dnk::~dnk() {
		delete first_rnk;
		delete second_rnk;
	}
    dnk::dnk(my_rnk& first_rnk, my_rnk& second_rnk){
        if (first_rnk.is_complimentary(second_rnk)){
			my_rnk *newf_rnk =new my_rnk (first_rnk);
			my_rnk *news_rnk = new my_rnk(second_rnk);
            this->first_rnk = newf_rnk;
            this->second_rnk = news_rnk;
        }
        else{
            this->first_rnk=nullptr;
            this->second_rnk = nullptr;
        }
    }
    const my_rnk& dnk::get_first_rnk() const {
        return *first_rnk;
    }
    const my_rnk& dnk::get_second_rnk() const {
        return *second_rnk;
    }
