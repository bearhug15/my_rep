#pragma once
#include "sort.h"
#include <algorithm>


/*struct {
	bool string_cmp(const std::string& a, const std::string& b) {
		int c = a.compare(b);
		if (c > 0)
			return true;
		else
			return false;
	}
} my_cmp;*/


void sort::execute() {
	if (text == nullptr || text[0] == nullptr) {
		throw conveyor_docking_error("input text is nullptr");
	}
	check_params();
	std::sort(text[0]->begin(), text[0]->end());//*/
}
void sort::set_text(std::vector<std::string>** text) {
	worker::text = text;
}
void sort::set_params(std::vector<std::string>* params) {
	worker::params = params;
}
void sort::check_params() {
	if (worker::params->size() != 1)throw(comand_params_error("wrong number of params"));
}