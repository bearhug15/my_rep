#pragma once

#ifndef LAB_DNK_NUCLEOTIDE_H
#define LAB_DNK_NUCLEOTIDE_H
#include <iostream>

enum class  nucleotide  { A=0, G=1, C=2, T=3 };
/*std::ostream& operator<<(std::ostream& os, nucleotide nuc)
{
	switch (nuc)
	{
	case nucleotide::A: os << "A"; break;
	case nucleotide::G: os << "G"; break;
	case nucleotide::C: os << "C"; break;
	case nucleotide::T: os << "T"; break;
	default: os.setstate(std::ios_base::failbit);
	}
	return os;
}*/
#endif //LAB_DNK_NUCLEOTIDE_H
