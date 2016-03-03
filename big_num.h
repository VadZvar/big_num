#ifndef __BIGNUM__
#define __BIGNUM__

#include <iostream>
#include <fstream>
#include <cstdint>
#include <stack>

#define ALLOC_ERR "ALLOCATION ERROR"
#define BBITS 32
#define BASE_MASK(N) ((base)1) << (((N % (BBITS)) << 1) - 1)
#define DBASE_MASK(N) ((dbase)1) << (((N % 64) << 1) - 1)
#define CAR_BORDER 50
#define MR_REL_PARAM 10

using namespace std;
typedef uint32_t base;
typedef uint64_t dbase;

class BigNumber {

public:
    base *bn, *en, *ba, *ea; //bn, en - begin & end number, ba, ea - begin & end area
public:
    BigNumber(size_t, base);//выделяю память длины len и заполняю каждую ячейку большого числа значением fill
    BigNumber(base);
    BigNumber(int);
    BigNumber(const BigNumber&);
    BigNumber(const string &str);
    BigNumber (base *, size_t, size_t);
    BigNumber & operator=(const BigNumber&);
    static BigNumber * get_degree_two(size_t, size_t);
    static void div_mod (const BigNumber& a, base b, BigNumber * q, base * r);
    static void div_mod (BigNumber&, BigNumber&, BigNumber *, BigNumber *);
    bool operator < (const BigNumber&) const;
    bool operator > (const BigNumber&) const;
    bool operator >= (const BigNumber&) const;
    bool operator <= (const BigNumber&) const;
    bool operator == (const BigNumber&) const;
    bool operator != (const BigNumber&) const;
    BigNumber & operator += (const BigNumber&);
    BigNumber & operator -= (const BigNumber&);
    BigNumber & operator *= (base);
    BigNumber & operator *= (BigNumber&);
    BigNumber operator + (const BigNumber&) const;
    BigNumber operator - (const BigNumber&) const;
    BigNumber operator * (const BigNumber&);
    BigNumber operator * (base);
    BigNumber operator / (base) const;
    BigNumber operator / (BigNumber&);
    BigNumber operator % (BigNumber&);
    BigNumber operator >> (int num) const;
    BigNumber operator << (int num) const;
    BigNumber & operator >>= (int num);
    BigNumber & operator <<= (int num);
    friend istream& operator >> (istream&, BigNumber&);
    friend ostream& operator << (ostream&, const BigNumber&);
    ~BigNumber();
//private:
    BigNumber();
    BigNumber sqr();
    BigNumber pow (BigNumber & degree, BigNumber & mod);
    BigNumber & barret (const BigNumber&, BigNumber&);
    BigNumber fmul (const BigNumber&);
    void fmul_car (const BigNumber&, BigNumber&);
    bool miller_rabin_test();
    bool is_prime();
    static BigNumber gen_prime (size_t bits);
    static BigNumber random (int length);
    static BigNumber gen_num_with_bits (size_t num);
    static BigNumber gen_num_with_bits (size_t num_base, size_t size_bits);
    static BigNumber gen_num_less_than (BigNumber & b);
    int Compare(const BigNumber&) const;
    static void ReSize(BigNumber&, size_t);
    BigNumber Usual_mul(BigNumber&);
    void usual_mul_mem (const BigNumber&, BigNumber&);
    void minus (const BigNumber&, BigNumber&);
    void plus (const BigNumber&, BigNumber&) const;
    size_t bitsize();
};

#endif
