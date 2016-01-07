#include "bignum.h"
#include <cstdlib>

#define N 1000
#define M 2000

int main(int argc, char * argv[]){
	BigInt a, b, c, d;
	int t = 100;
	int count = 0;
	srand(time(NULL));
	for(;t;--t){
		a = random(M);
		b = random(N);
		while(b == 0){
			b = random(M);
		}
		c = a / b;
		d = a % b;
		if(a == b * c + d && a - d == b * c && b > d){
			++count;
		}
	}

	std::cout << count  << std::endl;
	
	return 0;
}
