#include "pch.h"
#include "my_dnk.h"
#include <crtdbg.h>

TEST(Creation, creation) {
	my_rnk r1(nucleotide::A, 10);
	EXPECT_EQ(10, r1.capacity());
}
TEST(Creation, long_creation) {
	my_rnk r1(nucleotide::A, 700000000);
}
TEST(Creation,copy) {
	my_rnk r1(nucleotide::A, 10);
	my_rnk r2 = r1;
	EXPECT_TRUE(r1 == r2);
}

TEST(Arithmetic, summ) {
	my_rnk r1(nucleotide::A, 2);
	my_rnk r2(nucleotide::A, 1);
	r2 = r2 + nucleotide::A;
	EXPECT_TRUE(r1 == r2);
	my_rnk r3 = nucleotide::A + nucleotide::A;
	EXPECT_TRUE(r1 == r3);
}

TEST(Cardinality, one_nuc) {
	my_rnk r1 = nucleotide::A + nucleotide::A + nucleotide::G + nucleotide::C + nucleotide::A + nucleotide::C;
	EXPECT_EQ(3,r1.cardinality(nucleotide::A));
	EXPECT_EQ(1, r1.cardinality(nucleotide::G));
	EXPECT_EQ(2, r1.cardinality(nucleotide::C));
	EXPECT_EQ(0, r1.cardinality(nucleotide::T));
	
}
TEST(Cardinality, all_nucs) {
	my_rnk r1 = nucleotide::A + nucleotide::A + nucleotide::G + nucleotide::C + nucleotide::A + nucleotide::C;
	std::unordered_map<nucleotide, int> my_map = r1.cardinality();
	for (auto elem : my_map) {
		EXPECT_EQ(r1.cardinality(elem.first), elem.second) ;
	}
}

TEST(Modifying, trim) {
	my_rnk r1(nucleotide::A, 10);
	my_rnk r2(nucleotide::A, 5);
	r1.trim(5);
	EXPECT_TRUE(r1 == r2);
	r1.trim(7);
	EXPECT_TRUE(r1 == r2);
}
TEST(Modifying, split) {
	my_rnk r1 = nucleotide::A + nucleotide::A + nucleotide::G + nucleotide::C + nucleotide::A + nucleotide::C;
	my_rnk r2 = r1.split(3);
	my_rnk r3 = nucleotide::A + nucleotide::A + nucleotide::G;
	my_rnk r4 = nucleotide::C + nucleotide::A + nucleotide::C;
	EXPECT_TRUE(r1 == r3);
	EXPECT_TRUE(r2 == r4);
}
TEST(Modifying, replacement) {
	my_rnk r1 = nucleotide::A + nucleotide::A + nucleotide::G + nucleotide::C + nucleotide::A + nucleotide::T;
	my_rnk r2(nucleotide::A, 6);
	r2[2] = nucleotide::G;
	r2[3] = nucleotide::C;
	r2[5] = nucleotide::T;
	EXPECT_TRUE(r1 == r2);
}
TEST(Modifying, complimentary) {
	my_rnk r1 = nucleotide::A + nucleotide::A + nucleotide::G + nucleotide::C + nucleotide::A + nucleotide::T;
	my_rnk r2 = nucleotide::T + nucleotide::T + nucleotide::C + nucleotide::G + nucleotide::T + nucleotide::A;
	EXPECT_TRUE(r1.is_complimentary(r2));
	EXPECT_TRUE(r1!=r2);
	r2[0]= nucleotide::A;
	EXPECT_FALSE(r1.is_complimentary(r2));
}
TEST(Modifying, opposite) {
	my_rnk r1 = nucleotide::A + nucleotide::A + nucleotide::G + nucleotide::C + nucleotide::A + nucleotide::T;
	my_rnk r2 =!r1;	
	EXPECT_TRUE(r1.is_complimentary(r2));
}

int main(int argc, char**argv){
	::testing::InitGoogleTest(&argc,argv);
	RUN_ALL_TESTS();
	_CrtDumpMemoryLeaks(); //Будет небольшая утечка от Гугл тестов
	return 0;
}
