#pragma once
#include<exception>
#include<string>
class emplacement_error : public std::exception {
	std::string reason;
public:
	emplacement_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("Emplacement error because ") + reason).c_str(); }
	std::string so_what() const { return std::string("Emplacement error because ") + reason; }
	std::string get_reason() const { return reason; }
};
class shooting_out_of_field : public std::exception {
	std::string reason;
public:
	shooting_out_of_field(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("shooting out of field ") + reason).c_str(); }
	std::string so_what() const { return std::string("shooting out of field ") + reason; }
	std::string get_reason() const { return reason; }
};
class round_end : public std::exception {
	std::string reason;
public:
	round_end(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("Winner is ") + reason).c_str(); }
	std::string so_what() const { return std::string("Winner is ") + reason; }
	std::string get_reason() const { return reason; }
};
class parameters_error : public std::exception {
	std::string reason;
public:
	parameters_error(std::string reason) : reason(reason) {}
	const char* what() const { return (std::string("Parameters error because ") + reason).c_str(); }
	std::string so_what() const { return std::string("Parameters error because ") + reason; }
	std::string get_reason() const { return reason; }
};
class end : public std::exception {
	
public:
	end() {}
	const char* what() const { return "Game end "; }
	std::string so_what() const { return std::string("Game end ") ; }
};