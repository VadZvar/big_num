#include "big_num.h"

#define M 100
#define NUM 10

int main() {
	BigNumber a;
	for (int t = NUM; t > 0; --t) {
		a = BigNumber::gen_prime(M);
		std::cout << "a = " << a << std::endl;
        std::cout << (std::string)((a.is_prime())?("True"):("False")) << std::endl;
	}
	return 0;
}
