#include <big_num.h>

#define M 1000
#define N 1

int main () {
    BigNumber a(0);
    for (int t = N; t; --t) {
        a = BigNumber::gen_prime(M);
        std::cout << a << std::endl;
        std::cout << (std::string)((a.is_prime()) ? ("True") : ("False")) << std::endl;
    }
    return 0;
}
