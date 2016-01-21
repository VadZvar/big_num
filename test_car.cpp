#include <big_num.h>

int main () {
    BigNumber a(0), b(0), c(0);
    std::cin >> a >> b;
    std::cout << "a = " << a << std::endl << "b = " << b << std::endl << "c = " << a.fmul(b) << std::endl; 
    return 0;
}
