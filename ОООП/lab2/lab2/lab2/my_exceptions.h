#pragma once
#include <exception>
#include <string>
class conveyor_docking_error : std::exception {
	std::string reason;
public:
	conveyor_docking_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("1Failor in conveyor docking because ") + reason).c_str(); }
	std::string get_reason() const{ return reason;}
};
class comand_params_error : std::exception {
	std::string reason;
public:
	comand_params_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("2Failor in getting comand params because ") + reason).c_str(); }
};
class instruction_list_error : std::exception {
	std::string reason;
public:
	instruction_list_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("3incorrect instruction list because ") + reason).c_str(); }
};