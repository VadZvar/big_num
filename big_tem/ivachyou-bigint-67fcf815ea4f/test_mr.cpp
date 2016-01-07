#include "bignum.h"

#define M 1000
#define NUM 100

int main(){
	BigInt a(0);
	std::cin >>a;
	std::cout <<a << std::endl;
	std::cout << (std::string)((a.is_prime())?("True"):("False")) << std::endl;
	return 0;
}	
