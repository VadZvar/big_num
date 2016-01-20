#include <big_num.h>

int main () {
    BigNumber a(0), b(0), c(0);
    std::cin >> a >> b >> c;
    try {
        std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "c = " << c << std::endl << "z = " << a.pow(b, c) << std::endl;
    } catch (const char * z) {
        std::cout << z << std::endl;
    }
    return 0;
}
