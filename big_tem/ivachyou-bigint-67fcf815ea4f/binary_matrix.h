#include "types.h"
#include <vector>

class binary_matrix{
	private:
	std::vector<binary_row> rows;
	base m, n;
	public:
	binary_matrix(base mm = 0, base nn = 0);
	binary_matrix(binary_matrix & m);
	~binary_matrix();
	inline binary_row & operator[](int i);
	void add_row(binary_row &);
	void stepped_form();
	void special_stepped_form();
	uint32_t rank();
	binary_row get_solve(binary_row & r);
};

struct binary_row{
	std::vector<base> cols;
	inline base & operator[](int i){
		return cols[i];
	}
	void sub(binary_row & s){
		base p = (cols.size() >= s.cols.size())?s.cols.size():cols.size();
		for(base i = 0; i < p; ++i){
			cols[i] ^= s.cols[i];
		}
	}
	void swap(binary_row & s){
		base p = (cols.size() >= s.cols.size())?s.cols.size():cols.size();
		base tmp;
		for(base i = 0; i < p; ++i){
			tmp = s.cols[i];
			s.cols[i] = cols[i];
			cols[i] = tmp;
		}
	
	}
};
