#include<tuple>
#include<iostream>

template <class Ch, class Tr, size_t N,typename... args>
struct tuple_print {
	static void print(std::basic_ostream<Ch, Tr>& os, std::tuple< args...> const& tup) {
		tuple_print<Ch, Tr, N - 1, args...>::print(os,tup);
		if (N < sizeof...(args)) {
			os << ',';
		}
		os << std::get<N>(tup);
	}
};

template <class Ch, class Tr,typename... args>
struct tuple_print<Ch, Tr,0, args...> {
	static void print(std::basic_ostream<Ch, Tr>& os, std::tuple< args...> const& tup) {
		os << std::get<0>(tup);
	}
};

template<class Ch, class Tr, typename... args>
struct tuple_print<Ch, Tr, -1, args...> {
	static void print(std::basic_ostream<Ch, Tr>& os, const std::tuple<args...>& tup){}
};

template<class Ch, class Tr,typename... args >
std::ostream& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<args...> const& tup){
	tuple_print<Ch, Tr, sizeof...(args) - 1, args...>::print(os, tup);
	return os;
}

