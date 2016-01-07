#include <iostream>
#include "polynom.h"

int main(){
	polynom a, b, d;
	std::cin >> a >> b;
	d = a.gcd(b);
	std::cout << a << std::endl << b << std::endl << d << std::endl;
	return 0;
}
