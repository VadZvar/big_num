#include "big_num.h"

#define M 40
#define NUM 10

int main() {
	BigNumber a;
	for (int t = NUM; t > 0; --t) {
		a = BigNumber::gen_prime(M);
		std::cout << a << std::endl;
	}
	return 0;
}
