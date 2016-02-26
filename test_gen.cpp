#include "big_num.h"

#define M 100
#define NUM 10

int main(){
	BigNumber a;
	std::cout << "l = []" << std::endl;
	for(int t = NUM; t >0; --t){
		a = BigNumber::gen_prime(M);
		std::cout << "a = " <<hex<< a << std::endl << "l.append(a)" << std::endl;
	}
	std::cout <<std::endl << "for n in l:" << std::endl << "\t" << "print str(n.is_prime())" << std::endl;
	return 0;
}
