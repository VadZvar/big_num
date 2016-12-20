#include <big_num.h>

int main () {
    
    BigNumber_d d;
    BigNumber a = BigNumber::gen_num_with_bits(30);
    std::cout << "a = " << a << std::endl;
    *a.bn |= 1;
    std::cout << "new a = " << a << std::endl;
    if (a.ferma_with_shft(d)) {
        for (auto i = d.begin(); i != d.end(); ++i) {
		    std::cout << *((*i) -> num) << "**" << (*i) -> degree << std::endl;
	    }
    }
    return 0;

}
