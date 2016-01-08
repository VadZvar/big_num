#ifndef __BIGNUM__
#define __BIGNUM__

#include <iostream>
#include <cstdint>
#include <stack>

#define ALLOC_ERR "ALLOCATION ERROR"
#define BBITS 32
#define BASE_MASK(N) ((base)1) << (((N % (BBITS)) << 1) - 1)
#define DBASE_MASK(N) ((dbase)1) << (((N % 64) << 1) - 1)
using namespace std;
typedef uint32_t base;
typedef uint64_t dbase;

class BigNumber {

public:
    base *bn, *en, *ba, *ea; //bn, en - begin & end number, ba, ea - begin & end area
public:
    BigNumber(int, base);//выделяю память длины len и заполняю каждую ячейку большого числа значением fill
    BigNumber(base);
    BigNumber(const BigNumber&);
    BigNumber(const string &str);
    BigNumber & operator=(const BigNumber&);
    static void division_base (const BigNumber& a, base b, BigNumber * q, base * r);
    bool operator < (const BigNumber&);
    bool operator > (const BigNumber&);
    bool operator >= (const BigNumber&);
    bool operator <= (const BigNumber&);
    bool operator == (const BigNumber&);
    bool operator != (const BigNumber&);
    BigNumber & operator += (const BigNumber&);
    BigNumber & operator -= (const BigNumber&);
    BigNumber & operator *= (base);
    BigNumber operator + (const BigNumber&) const;
    BigNumber operator - (const BigNumber&) const;
    BigNumber operator * (BigNumber&);
    BigNumber operator * (base);
    BigNumber operator / (base) const;
    BigNumber operator >> (int num) const;
    BigNumber operator << (int num) const;
    friend istream& operator >> (istream&, BigNumber&);
    friend ostream& operator << (ostream&, BigNumber&);
    ~BigNumber();
//private:
    BigNumber();
    int Compare(const BigNumber&);
    static void ReSize(BigNumber&, size_t);
    BigNumber Usual_mul(BigNumber&);
    void minus (const BigNumber&, BigNumber&);
    void plus (const BigNumber&, BigNumber&) const;
};

#endif
