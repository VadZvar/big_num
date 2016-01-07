#include <iostream>
#include "bignum.h"

int main(int argc, char * argv[]){
	BigInt a = BigInt::gen_num_with_bits(80);	
	BigInt_D l;
	BigInt border("1234523451111");
	a.method_trial_div(l, border);
	for(auto it  = l.begin(); it != l.end(); ++it){
		std::cout << *((*it) -> num) << "**" << (*it) -> degree << " * ";
	}
	std::cout << std::endl;
	return 0;
}
