#pragma once
#include <exception>
#include <string>
class conveyor_docking_error : std::exception {
	std::string reason;
public:
	conveyor_docking_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("Failor in conveyor docking because ") + reason ).c_str(); }
	std::string so_what() const { return std::string("Failor in conveyor docking because ") + reason; }
	std::string get_reason() const{ return reason;}
};
class comand_params_error : std::exception {
	std::string reason;
public:
	comand_params_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("Failor in getting comand params because ") + reason).c_str(); }
	std::string so_what() const { return std::string("Failor in getting comand params because ") + reason; }
};
class instruction_list_error : std::exception {
	std::string reason;
public:
	instruction_list_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("Incorrect instruction list because ") + reason).c_str(); }
	std::string so_what() const { return std::string("Incorrect instruction list because ") + reason; }
};
class uses_exceeded : std::exception {
	std::string reason;
public:
	uses_exceeded(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("uses exceeded of ") + reason).c_str(); }
	std::string so_what() const { return std::string("uses exceeded of ") + reason; }
};