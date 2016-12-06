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
	BigNumber p = BigNumber::gen_prime(10);
	BigNumber g = BigNumber::prime_root(p);
	BigNumber a = BigNumber::gen_num_less_than(p);
	BigNumber d = a.discret_log(g, p);
	if (g.pow(d, p) == a) {
		std::cout << "correct" << std::endl;
	} else std::cout << "nope" << std::endl;


    return 0;
}
