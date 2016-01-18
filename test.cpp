#include <big_num.h>

int main () {
    BigNumber a(0), b(0), c(0);
    std::cin >> a >> b >> c;
    BigNumber e = a.pow(b,c);
    std::cout << e << std::endl;
    return 0;
}
