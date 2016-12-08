#include <big_num.h>

int main () {
  /*  BigNumber a = BigNumber::gen_num_with_bits(100);
    BigNumber_d div;
    std::cout << a << std::endl;
    div = a.factor();
    std::cout << a << "==";
    for (auto i = div.begin(); i != div.end(); ++i) {
        std::cout << *((*i) -> num) << "**" << (*i) -> degree << " * ";
    }
    std::cout << std::endl; */
	//BigNumber p = BigNumber::gen_prime(30);
    BigNumber p = 963920351;
    std::cout << "p = " << p << std::endl;
	BigNumber g = BigNumber::prime_root(p);
    std::cout << "g = " << g << std::endl;
	BigNumber a = BigNumber::gen_num_less_than(p);
    std::cout << "a = " << a << std::endl;
	BigNumber d = a.discret_log(g, p);
    std::cout << "d = " << d << std::endl;
    BigNumber r = g.pow(d, p);
    std::cout << "r = " << r << std::endl;
	if (r == a) {
		std::cout << "correct" << std::endl;
	} else std::cout << "not today" << std::endl;


    return 0;
}
