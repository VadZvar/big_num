#include <iostream>
#include "bignum.h"

#define M 3000
#define N 3000

int main(int argc, char * argv[]){
	int t = 100, count = 0, m, n;
	srand(time(NULL));
	BigInt a, b, c, d;
	for(int i = 0; i < t; ++i){
		m = rand() % 3000 + 1000;
		n = rand() % 3000 + 1000;
		a = BigInt::random(n);
		b = BigInt::random(m);
		try{
		c = a.fmul2(b);
	//	d = a.usual_mul(b);
	/*	if(d != c){
				std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "fmul = " << c << std::endl << " * = " << d << std::endl;
				break;
			} else {
				++count;
				std::cout << count << std::endl;
			}
		} catch(const char * e){
			std::cout << e << std::endl;
		}*/
	}
	/*if(count == t){
		std::cout << "Congratulations!!!!!!1111" << std::cout;
	} else {
		std::cout << "Sorry!!!1111" << std::cout;
	}*/
	return 0;
}
