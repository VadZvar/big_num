#include <iostream>
#include "polynom.h"

int main(){
	polynom a, b, c, d;
	std::cin >> a >> b;
	std::cout << a << std::endl << b << std::endl;
	c = a / b;
	d = a % b;
	std::cout << c << std::endl << d << std::endl;
	return 0;
}
