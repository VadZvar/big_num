#include <iostream>
#include "bignum.h"

int main(){
	BigInt a("76186573177060243");//= BigInt::gen_num_with_bits(100);
	std::cout << a << std::endl;
	BigInt b = a.ro_pollard();
	std::cout << b << std::endl;
	BigInt c = a / b;
	if(c * b == a){
		std::cout << "Right!" << std::endl;
	} else std::cout << "Nope!" << std::endl;
}
