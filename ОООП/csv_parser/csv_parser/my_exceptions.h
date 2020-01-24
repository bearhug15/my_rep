#pragma once
#include <exception>
#include <string>
class rows_error : std::exception {
	std::string reason;
public:
	rows_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("rows_error") + reason ).c_str(); }
	std::string so_what() const { return std::string("rows_error") + reason; }
	std::string get_reason() const{ return reason;}
};

class transf_error : std::exception {
	std::string reason;
public:
	transf_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("transf_error") + reason).c_str(); }
	std::string so_what() const { return std::string("transf_error") + reason; }
	std::string get_reason() const { return reason; }
};
