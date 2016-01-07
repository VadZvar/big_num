#include "bignum.h"

int main(){
	BigInt a(0), b(0), c(0);
	std::cin >> a >> b;
	std::cout << a << std::endl << b << std::endl;
	c = a % b;
	std::cout << c << std::endl;
}
