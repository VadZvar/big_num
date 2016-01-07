#include "bignum.h"

int main(){
	BigInt a = BigInt::gen_num_with_bits(80), border("123123124234235235235");
	BigInt_D d;
	std::cout << a <<  " == ";
	a.quadratic_sieve(d, border);
	for(auto i = d.begin(); i != d.end(); ++i){
		std::cout << *((*i) -> num) << "**" << (*i) -> degree << " * ";
	}
	std::cout << std::endl;
	return 0;
}
