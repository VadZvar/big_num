#include <iostream>
#include "polynom.h"

int main(){
	polynom a;
	std::cin >> a;
	if(a.is_irreductible_2()){
		std::cout << "True" << std::endl;
	} else {
		std::cout << "False" << std::endl;
	}
	return 0;
}
