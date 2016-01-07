#ifndef __BigIntH
#define __BigIntH

#include <iostream>
#include <string>
#include <limits>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <list>
#include <cassert>
#include <vector>
#include <queue>
#include "types.h"
#include <cstdint>
#include <algorithm>


#define DEF_OPT(Class,A) \
bool Class::operator A (const Class & b) const{\
        return (cmp(*this, b) A 0)?true:false;\
}

#define ALLOC_ERR "ALLOCATION ERROR"

using namespace std;


#define BBITS (std::numeric_limits<base>::digits)
#define DBASE_MASK ((dbase)1) << ((BBITS << 1) - 1)
#define CAR_BORDER 50
#define MR_REL_PARAM 10
#define TRIAL_BORDER_SIZE ((int)2)
#define ERAT_BORDER ((int)10000)
#define FERMA_BASE 10
#define B_STEP 100

#define TRIAL_BORDER 100
#define P_M_BORDER 3000

class BigInt {
public:
    base *bn;
    base *ba;
    base *ea;
    base *en;
public:
    BigInt(base b);
    BigInt(int b);
    BigInt(size_t, base fill = 0);
    BigInt(base *, size_t, size_t);
	BigInt & operator=(const BigInt &);
    BigInt operator+(const BigInt &) const;
    friend ostream& operator<<(ostream &, const BigInt &);
    ~BigInt();
    friend istream& operator>>(istream &, BigInt &);
    BigInt(const string &str);
    BigInt(const BigInt & num);
    BigInt operator-(const BigInt &) const;
    BigInt operator*(base);
    BigInt operator*(BigInt &);
    BigInt & operator+=(const BigInt &);
    BigInt & operator-=(const BigInt &);
    BigInt & operator*=(base);
    BigInt & operator*=(BigInt &);
    BigInt & operator/=(const BigInt &);
    BigInt & operator/=(base);
    BigInt & operator%=(const BigInt &);
    BigInt & operator%=(base);
    bool operator==(const BigInt &) const;
    bool operator!=(const BigInt &) const;
    bool operator<(const BigInt &) const;
    bool operator>(const BigInt &) const;
    BigInt operator >>(int num) const;
    BigInt operator <<(int num) const;
    BigInt & operator >>=(int num);
    BigInt & operator <<=(int num);
    BigInt operator /(BigInt &) const;
    BigInt operator /(base) const;
    BigInt operator %(BigInt &) const;
    base operator %(base) const;
	BigInt sqr();
	BigInt& barret(BigInt&, BigInt&);
	BigInt pow(const BigInt & degree, BigInt & mod);
	BigInt fmul(BigInt & b);
	BigInt usual_mul(BigInt & b);
	static BigInt* get_degree_of_two(size_t, size_t);
	BigInt();
	bool miller_rabin_primality_test();
	bool is_prime();
	static BigInt gen_prime(size_t bits);
	static BigInt gen_num_less_than(BigInt & b);
	static BigInt gen_num_with_bits(size_t num_base, size_t num_bits);
	static BigInt gen_num_with_bits(size_t num);
	static BigInt random(int length);
	void plus(const BigInt&, BigInt&) const;
	void minus(const BigInt&, BigInt&);
	void usual_mul_mem(BigInt&, BigInt&);
	BigInt fmul2(BigInt &);
	void fmul_car(BigInt &, BigInt &);
	void factor(BigInt_D & div);
	BigInt_D factor();
	static BigInt* next_d(BigInt*);
	bool method_trial_div(BigInt_D &, BigInt &);
	void clean();
	void quadratic_sieve(BigInt_D &, base);
	bool is_degree(BigInt &, base &);
	BigInt pow(base n, BigInt &);
	static std::vector<base> generate_base_for_qsieve(base k, BigInt & a);	
	BigInt sqrt();
	void sqrt(BigInt &, BigInt *);
	void root_n(BigInt & res, base n);
	base solve_qeq_mod_bprime(base a, base p);
	static base inverse_mod(base a, base p);
	void light_copy(BigInt&);
	static int jacobi_symb(BigInt & a, base p);
	static void div_mod(const BigInt & a, BigInt & b, BigInt * q, BigInt * r);
    static void div_mod(const BigInt & a, base b, BigInt * q, base  * r);
	BigInt discrete_log(BigInt & g, BigInt & p);
	void F_pollard(pol_tup & tup, BigInt & a, BigInt & g, BigInt & p, BigInt & n);
	BigInt gcd(BigInt & b);
	BigInt inverse_mod(BigInt & b);
	bool ferma_with_sift(BigInt_D & div);
	static std::vector<base> generate_base( base k);
	bool ro_pollard(BigInt_D &);
	bool p_minus_pollard(BigInt_D & div, base B, base b);
	BigInt pow(base n);	
static BigInt primitive_root_modulo(BigInt & p);	

	public:
	static void generate_base_less_border(std::vector<base> & primes, base B);
	static void gen_num_less_than(BigInt & a, BigInt & b);
	static void generate_base_for_qsieve(std::vector<base> & b, base k, BigInt & n);
	static bool factor_one(std::pair<BigInt *, BigInt_DP> &, std::vector<base> &, BigInt *);
	static base pow_base_mod(base, base, base);
	static void resize(BigInt &, size_t);
    static int cmp(const BigInt &, const BigInt &);
	static int jacobi_symb(base, base);
	static base solve_quadratic_eq(base, base);
};

struct BP{
	BigInt * num;
	uint degree;
	BP(BigInt * n = nullptr, uint deg = 0){
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

struct sieve{
	base p;
	base index;
	base n;
	base p_n;
	void pack(base ind, base pr, base deg, base pr_n){
		p = pr;
		index = ind;
		n = deg;
		p_n = pr_n;
	}
};

struct dvector{
	std::vector<std::pair<BigInt *, BigInt_DP>> b_plus;
	std::vector<std::pair<BigInt *, BigInt_DP>> b_minus;
	dvector(){
	}
	~dvector(){
		
	}
	std::pair<BigInt *, BigInt_DP> & operator[](int i){
		if( i >= 0){
			if(i > b_plus.size()){
				throw "out";
			}
			return b_plus[i];
		} else {
			i = -i;
			if(i > b_minus.size()){
				throw "out of range";
			}
			return b_minus[i];
		}
	}
	void push_back_right(std::pair<BigInt *, BigInt_DP> pr){
		b_plus.push_back(pr);
	}
	void push_back_left(std::pair<BigInt *, BigInt_DP> pr){
		b_minus.push_back(pr);
	}
	bool is_actual(sieve & s){
		if(s.index >= b_plus.size() && (s.p_n - 1 - s.index >= b_minus.size())){
			return false;
		} else return true;
	}
	int size(){
		return b_plus.size() + b_minus.size();
	}
	void sift(sieve & s);
	void stub_sift(sieve & s, BigInt *);
	
};

struct pol_tup{
	BigInt x;
	BigInt y;
	BigInt b;
	pol_tup():x(0),y(0),b(0){
	}
	pol_tup(BigInt & a, BigInt & z, BigInt & c):x(a), y(z), b(c){
	}
	pol_tup(base a, base z, base c):x(a), y(z), b(c){
	}
	~pol_tup(){}
};

/*
struct binary_eqn{
	std::vector<dbase> bases;
	binary_line(base length = BBITS << 1):len(length){
	}
	~binary_line(){}
	base len;
};

struct binary_eqns{
	std::vector<binary_eqn> lines;
	base len;
	binary_matrix(base length = BBITS << 1):len(length){
	}
	void add_line(binary_eqn & eq){
		lines.push_back(eq);
	}
	void to_stepped_form(){

	}
};
*/
#endif
