#include "big_num.h"

int main () {
    BigNumber n(0), a(0), b(0);
    std::cin >> a >> b;
    std::cout << a << std::endl << b << std::endl;
    n = a + b;
    std::cout << n << std::endl;
    return 0;
}
