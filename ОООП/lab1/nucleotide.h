#pragma once

#ifndef LAB_DNK_NUCLEOTIDE_H
#define LAB_DNK_NUCLEOTIDE_H
#include <iostream>

enum class  nucleotide  { A=0, G=1, C=2, T=3 };
std::ostream& operator<<(std::ostream& os, nucleotide nuc);
#endif //LAB_DNK_NUCLEOTIDE_H
