#include "bignum.h"

#define DEF_OPT(Class,A) \
bool Class::operator A (const Class & b) const{\
        return (cmp(*this, b) A 0)?true:false;\
}

#define ALLOC_ERR "ALLOCATION ERROR"

using namespace std;


#define BBITS (std::numeric_limits<base>::digits)
#define DBASE_MASK ((dbase)1) << ((BBITS << 1) - 1)

class polynom{
	base * bn;
	base * en;
	base * ea;
	base * ba = nullptr;
	static void div_mod(polynom &, polynom &, polynom *, polynom *);
	static void resize(polynom &, size_t);
	public:
	polynom();
	polynom(base i);
	polynom(base *, base *, base *, base *);
	polynom(base *, size_t, size_t);
	polynom(size_t, base);
	~polynom();
	polynom(const polynom & p);
	polynom & operator=(const polynom &);
	polynom operator+(polynom &);
	polynom & operator+=(polynom &);
	polynom operator-(polynom &);
	polynom & operator-=(polynom &);
	polynom operator*(polynom &);
	polynom & operator*=(polynom &);
	polynom operator/(polynom &);
	polynom operator%(polynom &);
	void clear();
	int cmp_degree(polynom & b);
	bool deg_less(polynom & b);
	bool deg_great(polynom & b);
	bool deg_eq(polynom & b);
	bool operator<(polynom &);
	bool operator>(polynom &);
	bool operator==(polynom &);
	bool operator!=(polynom &);
	polynom operator>>(int num) const;
	polynom operator<<(int num) const;
	polynom & operator>>=(int num);
	polynom & operator<<=(int num);
	friend istream & operator>>(istream & in, polynom & p);
	friend ostream & operator<<(ostream & out, const polynom & p);
	int get_deg();
	polynom gcd(polynom &);
	polynom pow(BigInt &, polynom &);
	polynom pow(base, polynom&);
	bool is_irreductible_1();
	bool is_irreductible_2();
};
