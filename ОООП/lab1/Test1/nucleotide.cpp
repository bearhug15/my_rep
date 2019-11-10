#pragma once
#include "pch.h"
#include "nucleotide.h"
std::ostream& operator<<(std::ostream& os, nucleotide nuc)
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
}
