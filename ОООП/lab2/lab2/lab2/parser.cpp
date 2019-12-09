#include"parser.h"
#include "my_exceptions.h"
#include <functional>

parser::parser() {}
parser::parser(std::string filename) {
	input.open(filename);
	try {
		parse_new_instruction_line();
	}
	catch (instruction_list_error ) {
		throw instruction_list_error("incorrect start");
	}

}
parser::~parser() {
	input.close();
}

bool parser::end_of_instr() {
	if (data == "csed") return true;
	return false;
}
void parser::start_of_instr() {
	if (data != "desc")throw instruction_list_error("incorrect start");
	this->parse_new_instruction_line();
}
void parser::parse_new_instruction_line() {
	std::getline(input,data);
	try {
		input.exceptions(input.failbit);
	}
	catch (const std::ios_base::failure & e) {
		throw instruction_list_error("incorrect end") ;
	}
	processed_data = 0;
}
long long parser::get_num() {
	long long num = stoll(data, &processed_data);
	if (num < 0) throw(comand_params_error("Wrong number"));
	return num;
}
std::vector<std::string> parser::get_params(validator* com_val){
	std::vector<std::string> params;/* = new std::vector<std::string>;*/
	size_t data_size = data.size();
	size_t baff_pos = 0;
	baff_pos = data.find_first_not_of(" ", processed_data);
	if (data[baff_pos] != '=') throw (comand_params_error("wrong syntax"));
	processed_data = data.find_first_not_of(" ",baff_pos+1);
	if (baff_pos == std::string::npos) throw (comand_params_error("can`t get comand"));
	baff_pos = data.find_first_of(" ", processed_data);
	std::string com =data.substr(processed_data,((baff_pos!=std::string::npos)?(baff_pos- processed_data):(--baff_pos,data_size)));
	com_val->validate_comand(&com);
	params.push_back(com);
	processed_data = data.find_first_not_of(" ", baff_pos + 1);
	while (processed_data != std::string::npos) {
		baff_pos = data.find_first_of(" ", processed_data);
		params.push_back(data.substr(processed_data, baff_pos != std::string::npos ? baff_pos - processed_data :(baff_pos-- ,data_size)));
		processed_data = data.find_first_not_of(" ", baff_pos + 1);
	}
	
	processed_data = 0;
	return params;
}
size_t parser::get_next_con_block_num() {
	size_t baff_pos =data.find_first_not_of(" ", processed_data);
	if (baff_pos == std::string::npos)throw(conveyor_docking_error("end of conveyor"));
	if ((data[baff_pos] != '-'|| data[baff_pos+1]!='>')&& processed_data != 0)throw (conveyor_docking_error("wrong conveyor construction"));
	if (data[baff_pos] == '-')baff_pos += 2;
	processed_data = data.find_first_not_of(" ", baff_pos);
	baff_pos= data.find_first_not_of("123456789",processed_data);
	if (processed_data == baff_pos) throw(conveyor_docking_error("wrong conveyor construction"));
	std::string num_data = data.substr(processed_data,baff_pos!=std::string::npos?baff_pos-processed_data:std::string::npos);
	processed_data = baff_pos;
	size_t num = stoll(num_data);
	return num;
}
std::string parser::get_conveyor() {
	return data;
}

parser& parser::operator++() {
	this->parse_new_instruction_line();
	return *this;
}