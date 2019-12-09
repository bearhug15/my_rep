#pragma once
#include "Worker.h"

class replace : public worker {
public:
	void virtual execute();
	void virtual set_text(std::vector<std::string>** text);
	void virtual set_params(std::vector<std::string>* params);
	void virtual check_params();
};