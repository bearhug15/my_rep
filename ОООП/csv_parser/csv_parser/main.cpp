#include"csv_parser.cpp"

int main() {
	std::ifstream input;
	input.open("for_lab.csv");
	//csv_parser<int, std::string, int> par(&input, 0);
	
	input.close();
	return 0;
}