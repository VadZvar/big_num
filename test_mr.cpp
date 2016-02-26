#include <big_num.h>

#define M 1000
#define N 1

int main () {
    BigNumber a(0);
    std::cin >> a; 
    std::cout << "a = " << a << std::endl;
    std::cout << (std::string)((a.is_prime()) ? ("True") : ("False")) << std::endl;
    return 0;
}
