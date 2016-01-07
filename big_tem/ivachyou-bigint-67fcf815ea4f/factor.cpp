#include <iostream>
#include "bignum.h"

int main(){
	BigInt a = BigInt::gen_num_with_bits(100);
	BigInt_D div;
	std::cout << a << std::endl;
	div = a.factor();
	std::cout << a << " == ";
	for(auto i = div.begin(); i != div.end(); ++i){
		std::cout << *((*i) -> num) << "**" << (*i) -> degree << " * ";
	}
	std::cout << std::endl;
	return 0;
}
