#include <iostream>
#include "polynom.h"

int main(){
	polynom a;
	std::cin >> a;
	if(a.is_irreductible_1()){
		std::cout << "True" << std::endl;
	} else {
		std::cout << "False" << std::endl;
	}
	return 0;
}
