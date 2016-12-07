#include <big_num.h>

int main() {
    int n;
    //std::cin >> n;
	BigNumber b = BigNumber::gen_num_with_bits(30);
	//BigNumber b = 734890318;
    std::cout << b << std::endl;
	BigNumber_d d =  b.factor();
	for (auto i = d.begin(); i != d.end(); ++i) {
		std::cout << *((*i) -> num) << "**" << (*i) -> degree << std::endl;
	}

	return 0;
}
