#include <iostream>
#include "big_num.h"

#define M 1000
#define N 1000

int main(int argc, char * argv[]){
	int t = 1000, count = 0, m, n;
	srand(time(NULL));
	BigNumber a, b, c, d;
	for(int i = 0; i < t; ++i) {
        m = rand() % M + 1;
        n = rand() % M + 1;
		a = BigNumber::random(m);
		b = BigNumber::random(n);
		try {
		c = a.fmul(b);
        d = a * b;
        //std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "c = " << c = a.fmul(b) << std::endl;
        //std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "c = " << d = a * b << std::endl;
        //std::cout << ((d == c) ? ("True") : ("False")) << std::endl;
        if (c == d) {
            std::cout << "True" << std::endl;
        }
        else {
            std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "c = " << a.fmul(b) << std::endl;
            std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "c = " << a * b << std::endl;
        
        }


		} catch(const char * e) {
			std::cout << e << std::endl;
		}
	}
	return 0;
}
