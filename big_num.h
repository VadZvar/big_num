#ifndef __BIGNUM__
#define __BIGNUM__

#include <iostream>
#include <fstream>
#include <cstdint>
#include <stack>
#include <list>
#include <vector>
#include <queue>

#define ALLOC_ERR "ALLOCATION ERROR"
#define BBITS 32
#define BASE_MASK(N) ((base)1) << (((N % (BBITS)) << 1) - 1)
#define DBASE_MASK(N) ((dbase)1) << (((N % 64) << 1) - 1)
#define CAR_BORDER 50
#define MR_REL_PARAM 10
#define TRIAL_BORDER_SIZE ((int)2)
#define TRIAL_BORDER 1000000
#define ERATE_BORDER ((int)10000)
#define FERMA_BASE 10
#define B_STEP 100
#define P_M_BORDER 3000

using namespace std;

typedef uint32_t base;
typedef uint64_t dbase;
typedef unsigned int uint;
struct BP;
typedef std::list<BP *> BigNumber_d;
struct pol_tup;
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
    static void div_mod (const BigNumber&, BigNumber&, BigNumber *, BigNumber *);
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
	base operator % (base) const;
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
    BigNumber pow (const BigNumber & degree, BigNumber & mod);
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
    BigNumber_d factor ();
    static BigNumber * next_d (BigNumber *);
    bool trial_div (BigNumber_d &, BigNumber &);
    void clean();
    BigNumber pow (base, BigNumber &);
    BigNumber pow (base);
    BigNumber sqrt();
    BigNumber gcd (BigNumber &);
    BigNumber inverse_mod (BigNumber &);
	BigNumber discret_log (BigNumber &, BigNumber &);
    void sqrt (BigNumber &, BigNumber *);
    void light_copy (BigNumber &);
    void f_pollard (pol_tup &, BigNumber &, BigNumber &, BigNumber &, BigNumber &);
    //BigNumber a(0), b(0), c(0);
    static std::vector<base> generate_base (base k);
    bool ro_pollard (BigNumber_d &);
    bool ferma_with_shft(BigNumber_d &);
    bool p_1_pollard (BigNumber_d &, base, base);
    static void generate_base_less_border(std::vector<base> &primes, base B);
    static int jac_simb(base, base);
    static int jac_simb(BigNumber &, base);
	static BigNumber prime_root(BigNumber &);
};

struct BP{
	BigNumber * num;
	uint degree;
	BP(BigNumber * n = nullptr, uint deg = 0){
		degree = deg;
		num = n;
	}
	~BP(){
		if(num){
			delete num;
		}
	}
	static bool cmp(BP * x, BP * y){
		return *(x -> num) < *(y -> num);
	}
};
bool cmpBP(BP * x, BP * y);


struct P{
	base num;
	uint degree;
	P(base n, uint deg = 0){
		degree = deg;
		num = n;
	}
	~P(){}
};

/*struct BP {
    BigNumber *num;
    uint degree;

    BP (BigNumber *n = nullptr, uint deg = 0) {
        degree = deg;
        num = n;
    }

    ~BP() {
        if (num) {
            delete num;
        }
    }
    static bool cmp (BP *x, BP *y) {
        return *(x -> num) < *(y -> num);
    }
};

struct pol_tup {
    BigNumber x, y, b;
    pol_tup():x(0), y(0), b(0) {}
    pol_tup(BigNumber & a, BigNumber & z, BigNumber &c):x(a), y(z), b(c) {}
    pol_tup(base a, base z, base c):x(a), y(z), b(c) {}
    ~pol_tup(){}
};*/

struct pol_tup{
	BigNumber x;
	BigNumber y;
	BigNumber b;
	pol_tup():x(0),y(0),b(0){
	}
	pol_tup(BigNumber & a, BigNumber & z, BigNumber & c):x(a), y(z), b(c){
	}
	pol_tup(base a, base z, base c):x(a), y(z), b(c){
	}
	~pol_tup(){}
};

#endif
