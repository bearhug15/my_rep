#pragma once
#include "pch.h"
#include "my_rnk.h"
#ifndef LAB_DNK_DNK_H
#define LAB_DNK_DNK_H

class dnk{
    my_rnk* first_rnk;
    my_rnk* second_rnk;
    dnk();
public:
	~dnk();
    dnk(my_rnk& first_rnk, my_rnk& second_rnk);

    const my_rnk& get_first_rnk()const ;
    const my_rnk& get_second_rnk()const ;
};

#endif //LAB_DNK_DNK_H
