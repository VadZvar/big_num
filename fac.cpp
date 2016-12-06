#include <big_num.h>

int main() {


	BigNumber b = BigNumber::gen_num_with_bits(100);
	std::cout << b << std::endl;
	BigNumber_d d =  b.factor();
	for (auto i = d.begin(); i != d.end(); ++i) {
		std::cout << *((*i) -> num) << "**" << (*i) -> degree << std::endl;
	}

	return 0;
}
