#pragma omce
#include "replace.h"


void replace::execute() {
	if (text == nullptr || text[0] == nullptr) {
		throw conveyor_docking_error("input text is nullptr");
	}
	check_params();
	std::string word1 = params[0][1];
	std::string word2 = params[0][2];
	size_t word1l = word1.size();
	size_t word2l = word2.size();
	for (size_t i = 0; i<text[0]->size(); i++) {
		size_t p = (text[0][0])[i].find(word1);
		while (p != std::string::npos) {
			(text[0][0])[i].replace(p,word1l,word2);
			p += word2l;
			p = (text[0][0])[i].find(word1, p);
		}
	}
}
void replace::set_text(std::vector<std::string>** text){
	worker::text = text;
}
void replace::set_params(std::vector<std::string>* params) {
	worker::params = params;
}
void replace::check_params() {
	if (worker::params->size() != 3)throw(comand_params_error("wrong number of params"));
}