#pragma once
#include "readfile.h"


void readfile::execute(){
	if (text == nullptr || text[0] != nullptr) {
		throw conveyor_docking_error("input text isn`t nullptr");
	}
	check_params();
	std::ifstream file(params[0][1].c_str());
	try {
		file.exceptions(file.failbit);
	}
	catch (const std::ios_base::failure & e) {
		throw  e;
	}
	std::vector<std::string> *new_text = new std::vector<std::string>;
	std::string baff_str;
	try {
		for (size_t i = 0; std::getline(file, baff_str); i++) {
			new_text[0].push_back(baff_str);
		}
	}
	catch (std::exception& a) {

	}
	text[0] = new_text;

}
void readfile::set_text(std::vector<std::string>** text) {
	worker::text = text;
}
void readfile::set_params(std::vector<std::string>* params) {
	worker::params = params;
}
void readfile::check_params() {
	if (worker::params->size() != 2)throw(comand_params_error("wrong number of params"));
}