#include <big_num.h>

int main () {
    BigNumber_d div;
    BigNumber p = BigNumber::gen_num_with_bits(50);
    std::cout << p << std::endl;
    bool d = p.ro_pollard(div);
    for (auto &x:div) {
        std::cout << *(x -> num) << std::endl;
    }
    std::cout << d << std::endl;
return 0;
}
