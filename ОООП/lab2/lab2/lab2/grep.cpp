#pragma once
#include "grep.h"

void grep::execute() {

	if (text == nullptr || text[0] == nullptr) {
		throw conveyor_docking_error("input text is nullptr");
	}
	check_params();
	std::vector<std::string>* new_text = new std::vector<std::string>;
	std::string word = params[0][1];
	for (size_t i = 0; i < (text[0])->size(); i++) {
		if ((text[0][0])[i].find(word) != std::string::npos) {
			new_text->push_back((text[0][0])[i]);
		}
	}
	delete text[0];
	text[0] = new_text;
}
void grep::set_text(std::vector<std::string>** text) {
	worker::text = text;
}
void grep::set_params(std::vector<std::string>* params) {
	worker::params = params;
}
void grep::check_params() {
	if (worker::params->size() != 2)throw(comand_params_error("wrong number of params"));
}