#include <iostream>
#include "polynom.h"

int main(){
	polynom a, b, c;
	BigInt deg(1);
	std::cin >> a >> deg >> b;
	std::cout << a << std::endl << deg << std::endl << b << std::endl;
	c = a.pow(deg, b);
	std::cout << c << std::endl;
	return 0;
}
