#include <iostream>
#include "bignum.h"

int main(){
	BigInt p = BigInt::gen_prime(45);
	std::cout << "p = " << p << std::endl;
	BigInt alpha = BigInt::primitive_root_modulo(p);
	std::cout << "alpha = " << alpha << std::endl;
	BigInt x = BigInt::gen_num_less_than(p);
	
	if(x == 0){
		std::cout << "you are lucky boy!" << std::endl;
		return -1;
	}
	std::cout << x << "log(" << alpha << "," << p  << ") == ";
	BigInt b = x.discrete_log(alpha, p);
	std::cout << b << std::endl;

	return 0;
}
