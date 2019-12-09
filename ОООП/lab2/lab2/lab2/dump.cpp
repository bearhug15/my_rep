#pragma once
#include "dump.h"

void dump::execute() {
	if (text==nullptr || text[0] == nullptr) {
			throw conveyor_docking_error("dump input text is nullptr");
	}
	check_params();
	std::ofstream file(params[0][1]);
	try {
		file.exceptions(file.failbit);
	}
	catch (const std::ios_base::failure & e) {
		throw  e;
	}
	char n = '\n';
	for (size_t i = 0; i < (text[0][0]).size(); i++) {
		file.write((text[0][0])[i].c_str(), (text[0][0])[i].length()); 
		file.write(&n, 1);
	}
	file.close();
}
void dump::set_text(std::vector<std::string>** text){
	worker::text = text;
}
void dump::set_params(std::vector<std::string>* params) {
	worker::params = params;
}
void dump::check_params() {
	if (worker::params->size() != 2)throw(comand_params_error("wrong number of params"));
}