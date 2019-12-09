#pragma once
#include "Worker.h"
#include <fstream>
class file_worker : public worker {	
	std::string filename;
public:
	file_worker(std::string filename) :worker() {}
	std::string get_filename() { return filename; }
};