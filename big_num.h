#ifndef __BIGNUM__
#define __BIGNUM__

#include <iostream>
#include <cstdint>

typedef uint32_t base;
typedef uint64_t dbase;

class BigNumber {
    base *bn, *en, *ba, *ea; //bn, en - begin & end number, ba, ea - begin & end area
    public:
    BigNumber(int, base);//выделяю память длины len и заполняю каждую базу длины fill, которую передаю в fill
    BigNumber(base);
    BigNumber(const BigNumber&);
    operator = (const BigNumber&);
    operator < (const BigNumber&);
    operator > (const BigNumber&);
    operator >= (const BigNumber&);
    operator <= (const BigNumber&);
    operator == (const BigNumber&);
    operator != (const BigNumber&);
    ~BigNumber();
    private:
    BigNumber();
    Compare(const BigNumber&);
};

#endif
