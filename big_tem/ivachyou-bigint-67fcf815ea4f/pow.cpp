#include "bignum.h"

#define N 1
#define M 100

int main(){
	BigInt a, b, c;
	bool lolo;
	std::string s;
	int count = 0, t1, t2, t3;
	srand(time(NULL));
	for(int t = N; t; --t){
		a = BigInt::random(M);
		b = BigInt::random(M);
		c = BigInt::random(M);
		try {
			std::cout << "a = "<< a << std::endl<<"b = " << b << std::endl<<"c = " << c <<  std::endl<< "e = " << a.pow(b,c) << std::endl << "print str(e == pow(a,b,c))"<<std::endl;
		} catch(const char * e){
			std::cout << e << std::endl;
		}
	}
	return 0;
}
