#include <big_num.h>

int main() {
    BigNumber_d div;
    BigNumber p = BigNumber::gen_num_with_bits(60);
    std::cout << p << std::endl;
    bool q = p.p_1_pollard(div,P_M_BORDER, 10);
    for (auto &x:div) {
        std::cout << *(x -> num) << std::endl;
    }
    std::cout << q << std::endl;

    return 0;
}
