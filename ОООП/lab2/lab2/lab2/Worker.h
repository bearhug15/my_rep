/*#ifndef _FSTREAM_
#define _FSTREAM_
#include <fstream>
#endif*/

#ifndef _WORKER_H_
#define _WORKER_H_
#pragma once
#include<iostream>
#include <string>
#include <vector>
#include "my_exceptions.h"
#include<fstream>
class worker  {
protected:
	std::vector<std::string>** text;
	std::vector<std::string>* params;
	void virtual check_params() = 0;
public:
	worker() : text(nullptr), params(nullptr) {}
	void virtual execute()=0;
	void virtual set_text(std::vector<std::string>** text) = 0;
	void virtual set_params(std::vector<std::string>* params) = 0;
};
#endif // !