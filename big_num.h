#ifndef __BIGNUM__
#define __BIGNUM__

#include <iostream>
#include <cstdint>

#define ALLOC_ERR "ALLOCATION ERROR"
#define BBITS 32
#define BASE_MASK(N) ((base)1) << (((N % (BBITS)) << 1) - 1)
#define DBASE_MASK(N) ((dbase)1) << (((N % 64) << 1) - 1)
typedef uint32_t base;
typedef uint64_t dbase;

class BigNumber {
    base *bn, *en, *ba, *ea; //bn, en - begin & end number, ba, ea - begin & end area
    public:
    BigNumber(int, base);//выделяю память длины len и заполняю каждую ячейку большого числа значением fill
    BigNumber(base);
    BigNumber(const BigNumber&);
    BigNumber(const string &str);
    BigNumber & operator = (const BigNumber&);
    bool operator < (const BigNumber&);
    bool operator > (const BigNumber&);
    bool operator >= (const BigNumber&);
    bool operator <= (const BigNumber&);
    bool operator == (const BigNumber&);
    bool operator != (const BigNumber&);
    BigNumber & operator += (const BigNumber&);
    BigNumber & operator -= (const BigNumber&);
    BigNumber & operator + (const BigNumber&);
    BigNumber & operator - (const BigNumber&);
    BigNumber & operator * (const BigNumber&);
    BigNumber & operator * (base);
    BigNumber operator / (base) const;
    BigNumber operator >> (const BigNumber&);
    BigNumber operator << (const BigNumber&);
    friend istream& operator >> (istream&, BigNumber&);
    friend ostream& operator << (ostream&, BigNumber&);
    ~BigNumber();
    private:
    BigNumber();
    void Compare(const BigNumber&);
    static void ReSize(BigNumber&, size_t);
    BigNumber Usual_mul(BigNumber&);
    void minus (const BigNumber&, BigNumber&);
    void plus (const Bignumber&, BigNumber&);
    static void division_base (const BigNumber&, base, BigNumber& *q, base *r);
};

#endif
