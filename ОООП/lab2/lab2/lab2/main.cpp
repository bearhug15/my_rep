#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG
#include<conio.h>
#include<crtdbg.h>
#include"workers.h"
#include<unordered_map>
void process_input(int argc, char** argv, std::pair< char, std::string>*  inpair,std::pair< char, std::string>*outpair,std::string* conveyor_file){
	switch (argc) {
			case 6: 
			{
				std::pair< char, std::string> baffpair = { ((std::string)argv[4])[1],(std::string)argv[5] };
				switch (baffpair.first) {
				case 'i':inpair->second=baffpair.second; break;
				case 'o':outpair->second = baffpair.second; break;
				default: std::cout << "incorrect input"; exit(0);
				}
			}
			case 4: 
			{
				std::pair< char, std::string> baffpair = { ((std::string)argv[2])[1],(std::string)argv[3] };
				switch (baffpair.first) {
				case 'i':if (inpair->second == " ")inpair->second = baffpair.second; else { std::cout << "incorrect input"; exit(0); }  break;
				case 'o':if (outpair->second == " ")outpair->second = baffpair.second; else { std::cout << "incorrect input"; exit(0); } break;
				default: std::cout << "incorrect input"; exit(0);
				}
			}
			case 2: *conveyor_file = argv[1]; break;
			default:if (argc != 2 && argc != 4 && argc != 6) {
				std::cout << "incorrect input";
				exit(0);
			}
	}

}

void create_map_of_workers(std::unordered_map<std::string, worker*>* my_map) {
	worker* read_ = new readfile;
	my_map[0]["readfile"] = read_;
	worker* write_ = new writefile;
	my_map[0]["writefile"] = write_;
	worker* dump_ = new dump;
	my_map[0]["dump"] = dump_;
	worker* sort_ = new sort;
	my_map[0]["sort"] = sort_;
	worker* replace_ = new replace;
	my_map[0]["replace"] = replace_;
	worker* grep_ = new grep;
	my_map[0]["grep"] = grep_;
}
void clear_map_of_workers(std::unordered_map<std::string, worker*>* my_map) {
	if (my_map == nullptr)return;
	delete my_map[0]["readfile"];
	my_map[0]["readfile"] = nullptr;
	delete my_map[0]["writefile"];
	my_map[0]["writefile"] = nullptr;
	delete my_map[0]["dump"];
	my_map[0]["dump"] = nullptr;
	delete my_map[0]["sort"];
	my_map[0]["sort"] = nullptr;
	delete my_map[0]["replace"];
	my_map[0]["replace"] = nullptr;
	delete my_map[0]["grep"];
	my_map[0]["grep"] = nullptr;
}

void clear_all(std::pair< char, std::string>* inpair, std::pair< char, std::string>* outpair, std::string* conveyor_file, parser* new_pars, std::unordered_map<size_t, std::vector<std::string>>* params_map, std::unordered_map<std::string, worker*>* workers_map, std::vector<std::string>* text) {
	inpair->~pair();
	outpair->~pair();
	conveyor_file->~basic_string();
	delete params_map;
	clear_map_of_workers(workers_map);
	delete workers_map;
	delete new_pars;
	delete text;
}
int main(int argc, char** argv) {
	_CrtMemState st1,st2,dif_st;
	_CrtMemCheckpoint(&st1);
	std::pair< char, std::string> inpair = {'i'," "};
	std::pair< char, std::string> outpair = {'o'," "};
	std::string conveyor_file;
	process_input(argc,argv,&inpair,&outpair,&conveyor_file);
	//conveyor_file = "in1.txt";
	parser* new_pars= nullptr;
	std::unordered_map<size_t, std::vector<std::string>> *params_map = new std::unordered_map<size_t, std::vector<std::string>>;
	std::unordered_map<std::string, worker*>* workers_map = new std::unordered_map<std::string, worker*>();
	create_map_of_workers(workers_map);
	std::vector<std::string>* params = nullptr;
	std::vector<std::string>* text = nullptr;
	validator my_validator{};
	my_validator.set_comand_map(workers_map);
	std::string conveyor;
	
	
	try {
		parser* pars = new parser(conveyor_file);
		new_pars = pars;
		
		pars->start_of_instr();
		size_t num = 0;

		
		while (!pars->end_of_instr()) {
			num = pars->get_num();
			if (params_map->find(num) != params_map->end()) throw instruction_list_error("two instructions with matching id");
			params_map[0][num] = pars->get_params(&my_validator);
			pars->parse_new_instruction_line();
		}

	}
	catch (const instruction_list_error& e) {
		std::cout << e.what();
		clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		exit(0);
	}
	catch (const comand_params_error & e) {
		std::cout << e.what();
		clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		exit(0);
	}
	catch (_exception) {
		std::cout << "Something went wrong in parsing instructions";
		clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		exit(0);
	}
	
	try {
		new_pars->parse_new_instruction_line();
	}
	catch (instruction_list_error) {
		std::cout << "conveyor not exists";
		clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		exit(0);
	}
	
	try {
		if (inpair.second != " "){
			try{
				worker* current_worker = workers_map[0]["readfile"];
				current_worker->set_text(&text);
				std::vector<std::string>vec = { "readfile", inpair.second };
				current_worker->set_params(&vec);
				current_worker->execute();
			}
			catch (const conveyor_docking_error & e) {
				std::cout << e.what();
				clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
				exit(0);
			}
			
		}
		conveyor = new_pars->get_conveyor();
		my_validator.validate_con(&conveyor,params_map);
		while (true) {
			size_t num = (*new_pars).get_next_con_block_num();
			params= &(params_map[0][num]);
			worker* current_worker = workers_map[0][params[0][0]];
			current_worker->set_text(&text);
			current_worker->set_params(params);
			current_worker->execute();
		}
	}
	catch (const conveyor_docking_error& e) {
		if (e.get_reason() == std::string("end of conveyor")){
			try{
				if (outpair.second != " "){
					worker* current_worker = workers_map[0]["writefile"];
					current_worker->set_text(&text);
					std::vector<std::string>vec = { "writefile", inpair.second };
					current_worker->set_params(&vec);
					current_worker->execute();

				}
			}
			catch (const conveyor_docking_error& e){
				std::cout << e.what();
				clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
				exit(0);
			}
			
			clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		}
		else{
			std::cout << e.what();
			clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
			exit(0);
		}
	}
	catch (const std::ios_base::failure & e) {
		std::cout << "can`t open file";
		clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		exit(0);
	}
	catch (const comand_params_error & e) {
		std::cout << e.what();
		clear_all(&inpair, &outpair, &conveyor_file, new_pars, params_map, workers_map, text);
		exit(0);
	}
	//*/

	//clear_all(&inpair,&outpair,&conveyor_file,new_pars,params_map,workers_map,text);
	/*inpair.~pair();
	outpair.~pair();
	conveyor_file.~basic_string();
	delete params_map;
	clear_map_of_workers(workers_map);
	delete workers_map;
	delete new_pars;
	delete text;*/
	
	_CrtMemCheckpoint(&st2);
	_CrtMemDifference(&dif_st, &st1, &st2);
	_CrtMemDumpStatistics(&dif_st);//*/
	return 0;
}