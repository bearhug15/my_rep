#pragma once
#include "writefile.h"


void writefile::execute() {
	if (text == nullptr || text[0] == nullptr) {
			throw conveyor_docking_error("input text is nullptr");
	}
	check_params();
	std::ofstream file(params[0][1]);
	try {
		file.exceptions(file.failbit);
	}
	catch (const std::ios_base::failure & e) {
		throw  e;
	}
	for (size_t i = 0; i<(text[0][0]).size();i++) { file.write((text[0][0][i]+std::string("\n")).c_str(), (text[0][0])[i].length()+ (long)1); }
	for (size_t i = 0; i<(text[0][0]).size(); i++) { text[0][0][i].~basic_string(); }
	delete text[0];
	text[0] = nullptr;
	set_text(nullptr);
	file.close();
}
void writefile::set_text(std::vector<std::string>** text) {
worker::text = text;
}
void writefile::set_params(std::vector<std::string>* params) {
	worker::params = params;
}
void writefile::check_params() {
	if (worker::params->size() != 2)throw(comand_params_error("wrong number of params"));
}