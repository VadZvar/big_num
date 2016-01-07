#include "bignum.h"

#define N 100
#define M 1

int main(int argc, char * argv[]){
	BigInt a, b, c;
	bool lolo;
	std::string s;
	int count = 0, t1, t2, t3;
	srand(time(NULL));
	for(int t = N; t; --t){
		a = random(M);
		b = a * a;
		c = a.sqr();
		std::cout << "b = " << b << std::endl << "c = " << c << std::endl;
		if(b == c){
			++count;
		}
	}
	std::cout << count << std::endl;
	return 0;
}
