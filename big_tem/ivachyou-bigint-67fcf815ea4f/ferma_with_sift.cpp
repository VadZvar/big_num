#include <iostream>
#include "bignum.h"

int main(){
	for(int l = 0; l < 1; ++l){
		BigInt a("4015171672247459");/* = BigInt::gen_num_with_bits(30);
		BigInt b = BigInt::gen_num_with_bits(30);
				a *= b;
		*/
		BigInt_D div;

		while(a % 2 == 0){
			a >>= 1;
		}
		std::cout << a << std::endl;
		if(a.ferma_with_sift(div)){
			std::cout  << a << " is prime " << std::endl;
		} else {
			std::cout << a << " == ";
			for(auto i = div.begin();i != div.end(); ++i){
				std::cout << *((*i) -> num) <<"**" << (*i) -> degree << " * ";
			}
			std::cout << std::endl;
		}
	}
	return 0;
}
