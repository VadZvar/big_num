#include <iostream>
#include "bignum.h"

int main(){
	BigInt a = BigInt::gen_num_with_bits(80);
	BigInt_D div;
	std::cout << a << std::endl;
	a.p_minus_pollard(div, 200, 10);
	std::cout << a << " == ";
	for(auto i = div.begin(); i != div.end(); ++i){
		std::cout << *((*i) -> num) << "**" << (*i) -> degree << " * ";
	}
	std::cout << std::endl;
	return 0;
}
