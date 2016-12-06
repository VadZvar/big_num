#include <big_num.h>

int main () {
    BigNumber a = BigNumber::gen_num_with_bits(100);
    BigNumber_d div;
    std::cout << a << std::endl;
    div = a.factor();
    std::cout << a << "==";
    for (auto i = div.begin(); i != div.end(); ++i) {
        std::cout << *((*i) -> num) << "**" << (*i) -> degree << " * ";
    }
    std::cout << std::endl;
    return 0;
}
