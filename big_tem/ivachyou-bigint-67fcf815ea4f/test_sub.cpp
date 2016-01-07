#include "bignum.h"

#define M 1000

int main(){
	int t = M, count = 0, m, n;
	BigInt a, b, c;
	srand(time(NULL));
	for(int i = t; i > 0; --i){
		m = rand() % 1000 + 100;
		n = rand() % 1000 + 100;
		a = BigInt::random(m);
		b = BigInt::random(n);
		if(b > a){
			c = b;
			b = a;
			a = c;
		}
		c = a - b;
		a -= b;
		if(a == c){
			count++;
		} else {
			std::cout << a << " " << b << " " << c << std::endl;
			break;
		}
	}
	return 0;
}
