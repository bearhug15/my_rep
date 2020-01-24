#include<iostream>
#include<fstream>
#include<tuple>
#include<string>
#include"tuple_print.cpp"
#include"my_exceptions.h"
#include<typeinfo>
#include<vector>
#include<sstream>

//#define cvar(var) "var";
//#define transf(var) "var";

/*template<typename arg,typename... args>
arg min(arg first, args... other) {
	arg buff = min(other...);
	return (first<buff?first:buff);
}
template<typename arg>
arg min(arg first) {
	return first;
}

template<typename... args>
class csv_parser {
	std::ifstream* input;
	std::tuple<args...> tup;
	char shielder = '"';
	char rows_sep = '\n';
	char col_sep = ',';
	std::string data;
	size_t data_processed = 0;
	size_t number_of_args = 0;
	size_t col_gained = 0;
	size_t rows_gained = 0;
	bool is_last_col = false;

	template<typename arg>
	arg get_data(arg varg){
		col_gained++;
		if (is_last_col) {
			std::cout<<"Column number "<<col_gained<<" not exists"<<std::endl;
			exit(0);
		}
		
		std::string sbuff="";
		size_t new_proc = 0;
		size_t shbuff = 0, rbuff = 0, cbuff = 0, min_buff=0;
		do {
			shbuff = data.find_first_of(shielder, data_processed);
			rbuff = data.find_first_of(rows_sep, data_processed);
			cbuff = data.find_first_of(col_sep, data_processed);
			new_proc = min(shbuff, rbuff, cbuff);
			if (new_proc == shbuff && new_proc!=std::string::npos) {
				sbuff += data.substr(data_processed, new_proc- data_processed);
				data_processed = new_proc;
				sbuff+= data.substr(data_processed+1, 1);
				data_processed += 2;

			}
			else {
				sbuff += data.substr(data_processed, new_proc-data_processed);
				data_processed = new_proc;
			}
		} while (new_proc == shbuff);
		if (new_proc == std::string::npos) {
			is_last_col = true;
		}
		const size_t harg = typeid(arg).hash_code();
		const size_t hint = typeid(int).hash_code();

		try {
			switch (harg) {
			case hint: varg = stoi(sbuff); break;
			case typeid(long).hash_code():
			case typeid(long long).hash_code(): varg = stol(sbuff); break;
			case typeid(double).hash_code(): varg = stod(sbuff); break;
			case typeid(float).hash_code(): varg = stof(sbuff); break;
			case typeid(char).hash_code(): varg = sbuff[0]; break;
			case typeid(std::string).hash_code(): return (arg)sbuff; break;
			default:throw transf_error("");//*varg =arg(sbuff);
			}
		}
		catch (std::exception) {
			std::cout << "Error in converting types in coloumn "<< col_gained<<std::endl;
			exit(0);
		}
		return varg;
	}

public: 
	csv_parser(std::ifstream* new_stream, size_t excess_rows) {
		number_of_args = sizeof...(args);
		input = new_stream;
		try {
			for (int i = 0; i < excess_rows; i++)std::getline(*input, data, rows_sep);
			rows_gained++;
			if (input->fail()) { throw rows_error(""); }
		}
		catch (std::exception) {
			std::cout <<"Failed in skipping "<<rows_gained<<" row. "<<"Not enough rows"<<std::endl;
			exit(0);
		}
		//
		std::getline(*input, data, rows_sep);
		tup = std::make_tuple((get_data(args(0)))...);
		data_processed = 0;
		col_gained = 0;
		is_last_col = false;
		//
	}

	csv_parser<args...>& operator++ () {
		std::getline(*input, data, rows_sep);
		tup = std::make_tuple(get_data(args(0))...);
		data_processed = 0;
		col_gained = 0;
		is_last_col = false;
		return *this;
	}

	std::tuple<args...>const& operator*() const { return tup; };
	std::tuple<args...>const* operator->() const { return &tup; };

	bool operator == (csv_parser<args...> second) {
		return ((this == &second) || (this->input == NULL) && (second->input ==NULL));
	}



	typedef std::input_iterator_tag     iterator_category;
	typedef std::tuple<args...>         value_type;
	typedef std::size_t                 difference_type;
	typedef std::tuple<args...>*        pointer;
	typedef std::tuple<args...>&        reference;
};*/

#define STO(var,v) void operator()(var & n) {\
							try{\
								if (data[0][col_gained].size() == 0) {\
									n = 0;\
								}\
								else {\
									n = std::sto##v (data[0][col_gained]);\
								}\
								col_gained++;\
							}\
							catch(std::exception){\
								std::cout<<"Error in conversion in "<<col_gained<<" column";\
							}\
					}

template<typename... args>
class csv_row
{
public:

	csv_row() {}
	csv_row(char col_sep, char rows_sep, char shielder) :col_sep(col_sep), rows_sep(rows_sep), shielder(shielder) {}
	struct converter
	{
		
		size_t col_gained = 0;
		std::vector<std::string>* data;
		converter(std::vector<std::string>* data) : data(data) {}
		
		void operator()(int& n)
		{
			std::cout << "int"<<std::endl;
			//n *= 2;
			col_gained++;
		}
		void operator()(std::string& s)
		{
			std::cout << "string" << std::endl;
			//s = data[0][col_gained];
			col_gained++;
		}
		void operator()(char& s)
		{
			if (data[0][col_gained].size() == 0) {
				s = ' ';
			}
			else {
				s = data[0][col_gained][0];
			}
			col_gained++;
		}
		STO(double, d)
		STO(float, f)
		STO(long, l)
		STO(long long, l)
		//STO(int, i)
	};

	template <class Tuple, class F, std::size_t... I>
	constexpr F for_each_impl(Tuple&& t, F&& f, std::index_sequence<I...>)
	{
		return (void)std::initializer_list<int>{(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))), 0)...}, f;
	}

	template <class Tuple, class F>
	constexpr F for_each(Tuple&& t, F&& f)
	{
		return for_each_impl(std::forward<Tuple>(t), std::forward<F>(f),
			std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
	}
	
	std::vector<std::string> make_data(size_t number, std::stringstream* line_stream) {
		std::vector<std::string> new_data;
		for (; col_gained < number;) {
			col_gained++;
			if (line_stream->eof()) {
				std::cout << "Column number " << col_gained << "  not excists";
				exit(0);
			}
			std::string cell;
			std::string buff_cell;
			bool again = true;
			while (again) {
				std::getline(*line_stream, buff_cell, col_sep);
				if (buff_cell.size() > 0) {
					if (buff_cell[buff_cell.size() - 1] == shielder) {
						again = true;
						buff_cell = buff_cell.substr(0, buff_cell.size() - 1) + col_sep;
					}
					else {
						again = false;
					}
					cell += buff_cell;
				}
			}
			new_data.push_back(cell);
		}
	}

	void readNextRow(std::istream& str){
		col_gained = 0;
		rows_gained++;
		std::string line="";
		std::string buff_line ="";
		bool again = true;
		try {
			while (again){
				again = false;
				std::getline(str, buff_line, rows_sep);
				if (buff_line.size() > 0 && buff_line[buff_line.size()-1]==shielder) {
					line += buff_line.substr(0,buff_line.size() - 1) + rows_sep;
					again = true;
				}
				else {
					line += buff_line;
				}
			}
		}
		catch (std::exception) {
			std::cout << "Error in getting "<< rows_gained<<" row";
			exit(0);
		}

		std::stringstream   line_stream(line);
		data = make_data(sizeof...(args),&line_stream);
		col_gained = 0;
		for_each(tup, converter{&data});

	}

	std::tuple<args...>* get_tuple() {
		return &tup;
	}

private:
	std::vector<std::string> data;
	std::tuple<args...> tup;
	char shielder = '"';
	char rows_sep = '\n';
	char col_sep = ',';
	size_t data_processed = 0;
	size_t number_of_args = 0;
	size_t col_gained = 0;
	size_t rows_gained = 0;
	bool is_last_col = false;
	//std::stringstream   lineStream;
	friend struct converter;
};

template<typename... args>
std::istream& operator>>(std::istream& str, csv_row<args...>& data)
{
	data.readNextRow(str);
	return str;
}

template<typename... args>
class csv_parser
{
public:
	typedef std::input_iterator_tag     iterator_category;
	typedef std::tuple<args...>         value_type;
	typedef std::size_t                 difference_type;
	typedef std::tuple<args...>*        pointer;
	typedef std::tuple<args...>&        reference;

	csv_parser<args...> begin() {
		return *this;
	}

	csv_parser<args...> end() {
		csv_parser<args...> pars(NULL, 0);
		return pars;
	}
	csv_parser(std::istream* str, size_t skip) { 
		if (str->eof()) {
			std::cout << "Eof";
			exit(0);
		}
		if (str->bad()) {
			std::cout << "Bad file";
			exit(0);
		}
		if (str->fail()) {
			std::cout << "Failed in opening";
			exit(0);
		}
		m_str = str;
		for (size_t i = 0; i < skip + 1; i++) { 
			++(*this);
		}
	}
	csv_parser(std::istream* str, size_t skip, char col_sep, char rows_sep, char shielder): m_row(col_sep,rows_sep, shielder)  {
		if (str->eof()) {
			std::cout << "Eof";
			exit(0);
		}
		if (str->bad()) {
			std::cout << "Bad file";
			exit(0);
		}
		if (str->fail()) {
			std::cout << "Failed in opening";
			exit(0);
		}
		m_str = str;
		for (size_t i = 0; i < skip + 1; i++) {
			++(*this);
		}
	}
	csv_parser() :m_str(NULL) {}

	// Pre Increment
	csv_parser& operator++() { 
		if (m_str) { 
			if (!((*m_str) >> m_row)) { 
				m_str = NULL; 
			} 
		}
		return *this; 
	}
	// Post increment
	csv_parser operator++(int) { csv_parser    tmp(*this); ++(*this); return tmp; }
	std::tuple<args...>& operator*() { return *(m_row.get_tuple()); }
	std::tuple<args...> const* operator->()  const { return m_row.get_tuple(); }

	bool operator==(csv_parser const& rhs) { return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL))); }
	bool operator!=(csv_parser const& rhs) { return !((*this) == rhs); }
private:
	std::istream*       m_str;
	csv_row<args...>    m_row;
};

int main()
{
	std::ifstream file;
	file.open("test.csv");

	csv_parser<int, int, std::string> par(&file, 0);
	/*for (std::tuple<int, std::string, int> i : par) {
		std::cout << i<< std::endl;
	}*/
}
