#include <big_num.h>

int main () {
    BigNumber a(0), b(0), c(0);
    std::cin >> a >> b;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    c = a * b;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
