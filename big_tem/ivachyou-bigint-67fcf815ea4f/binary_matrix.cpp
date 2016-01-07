#include "binary_matrix.h"

binary_matrix::binary_matrix(base nn, base mm):n(nn), m(mm){
	binary_row r;
	base m_c = m / BBITS + 1;
	for(base j = 0; j < m_c; ++j){
		r.cols.push_back(0);
	}

	for(base i = 0; i < n; ++i){
		rows.push_back(r);
	}
}

binary_matrix::binary_matrix(binary_matrix & matr){
	for(auto i = matr.rows.begin(); i != matr.rows.end(); ++i){
		rows.push_back(*i);
	}
	n = matr.n;
	m = matr.m;
}

binary_matrix::~binary_matrix(){}

void binary_matrix::add_row(binary_row & r){
	rows.push_back(r);
	n++;
}

binary_row & binary_matrix::operator[](int i){
	return rows[i];
}

void binary_matrix::special_stepped_form(){
	base i,j, mask = ((base)1) << (BBITS - 1);
	for(i = 0;i < m; ++i){
		for(j = 0; j < n; ++j){
			if(rows[j] & mask){
				rows[i].swap(rows[j]);
				break;
			}
		}
		if(j != n){
			for(base k = 0;k < n; ++k){
				if(k != i){
					rows[k].sub(rows[i], i);
				}
			}
		}
		mask >>= 1;
		if(!mask){
			mask = ((base)1) << (BBITS - 1);
		}
	}
}


