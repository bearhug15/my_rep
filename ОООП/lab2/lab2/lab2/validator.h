#pragma once
#include<string>
#include<unordered_map>
#include"Worker.h"
class validator {
	std::unordered_map< std::string, worker*>* comand_map;
public:
	validator();
	void virtual validate_con(std::string* data, std::unordered_map<size_t,std::vector<std::string>>*);
	void virtual validate_comand(std::string* com);
	void virtual set_comand_map(std::unordered_map<std::string,worker*>*);
};