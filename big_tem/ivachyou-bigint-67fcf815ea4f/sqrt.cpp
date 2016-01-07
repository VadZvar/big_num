#include <iostream>
#include "bignum.h"

int main(){
	int count = 0;
	for(int i = 0;i < 1000; ++i){
		BigInt a = BigInt::gen_num_with_bits(100);
		BigInt b = a.sqrt();
		if(a > b * b && a < (b + 1).sqr()){
			std::cout << "success" << i << std::endl;
		} else {count++;}
	}
	std::cout << "total faults " << count << std::endl;
	return 0;
}
