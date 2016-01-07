#include "polynom.h"

polynom::polynom(){
	ba = bn = ea = en = nullptr;
}

polynom::polynom(base i){
	base t = i / BBITS;
	bn = ba = new base[t + 1];
	en = ea = ba + t;
	for(base * p1 = ba;p1 <= ea;++p1){
		*p1 = 0;
	}
	*ea |= 1 << (i - t * BBITS); 
}

polynom::polynom(base * baa, base * bnn, base * eaa, base * enn){
	ba = baa;
	bn = bnn;
	en = enn;
	ea = eaa;
}

polynom::polynom(base * p, size_t real_size, size_t mem_size){
	ba = bn = p;
	en = p + real_size - 1;
	ea = p + mem_size - 1;
}

polynom::polynom(size_t size, base fill){
	bn = ba = new base[size];
	ea = ba + size - 1;
	for(base * tmp = ba; tmp <= ea; ++tmp){
		*tmp = fill;
	}

	if(fill){
		en = ba + size - 1;
	} else{
		en = ba;
	}
}

polynom::~polynom(){
	if(ba){
		delete[] ba;
	}
}

polynom::polynom(const polynom & p){
	size_t size_pol = p.en - p.bn + 1;
	base * p1, * p2; 
	if(ba){
		size_t size_now = ea - ba + 1;
		if(size_pol > size_now){
			delete[] ba;
			ba = bn = new base[size_pol];
			en = ea = ba + size_pol - 1;
		} else {
			bn = ba;
			en = ea;
		}
	} else {
		bn = ba = new base[size_pol];
		en = ea = ba + size_pol - 1;
	}
	
	for(p1 = bn;p1 <= en;*p1 = 0, ++p1);
	for(p1 = bn, p2 = p.bn; p1 <= en; ++p1, ++p2){
		*p1 = *p2;
	}
	en = bn + size_pol - 1;
}

polynom & polynom::operator=(const polynom & p){
	size_t size_pol = p.en - p.bn + 1;
	base * p1, * p2; 
	if(ba){
		size_t size_now = ea - ba + 1;
		if(size_pol > size_now){
			delete[] ba;
			ba = bn = new base[size_pol];
			en = ea = ba + size_pol - 1;
		} else {
			bn = ba;
			en = ea;
		}
	} else {
		bn = ba = new base[size_pol];
		en = ea = ba + size_pol - 1;
	}
	
	for(p1 = bn;p1 <= en;*p1 = 0, ++p1);
	for(p1 = bn, p2 = p.bn; p1 <= en; ++p1, ++p2){
		*p1 = *p2;
	}
	en = bn + size_pol - 1;
	return *this;
}

void polynom::resize(polynom & b, size_t new_size){
	int i;
    size_t size_b = b.en - b.bn + 1;
    if(new_size <= 0) {
        std::cout << "resize" << std::endl << std::flush;
		throw "bad size";
    }
    if(new_size > size_b) {
        if(new_size > b.ea - b.ba + 1) {
            base * a = new base[new_size];
            if(!a) {
				std::cout << "resize 2" << std::endl << std::flush;
                throw ALLOC_ERR;
            }
            for(i = 0; i < size_b; i++) {
                a[i] = b.bn[i];
            }
            for(; i < new_size; i++) {
                a[i] = 0;
            }

            delete[] b.ba;
            b.ba = b.bn = a;
            b.en = a + size_b - 1;
            b.ea = a + new_size - 1;
        } else {
			if(b.en - b.ba + 1 >= new_size){
				size_t diff = new_size - size_b;
				base * beg = b.bn - diff;
            	for(i = 0; i < size_b; i++) {
            	    beg[i] = b.bn[i];
            	}
            	for(; i < new_size; i++) {
            	    beg[i] = 0;
            	}
            	b.bn = beg;
				for(base * tmp = beg + new_size; tmp <= b.ba; *tmp = 0, ++tmp);
           		b.en = beg + size_b - 1;
			} else {
				for(i = 0; i < size_b; i++) {
            	    b.ba[i]	 = b.bn[i];
            	}
            	for(; i < new_size; i++) {
            	    b.ba[i] = 0;
            	}
            	b.bn = b.ba;
				for(base * tmp = b.ba + new_size; tmp <= b.ba; *tmp = 0, ++tmp);
           		b.en = b.ba + size_b - 1;
			}
        }
    } else {
        b.en = b.bn + new_size - 1;
    }
	for(;b.en == 0 && b.en > b.bn; --b.en);
}

int polynom::cmp_degree(polynom & b){
	size_t size_a = en - bn + 1;
	size_t size_b = b.en - b.bn + 1;
	if(size_a > size_b){
		return 1;
	} else if(size_b > size_a){
		return -1;
	}
	if(*en == *b.en){
		return 0;
	}
	base mask = ((base)1)<<(BBITS - 1);
	for(;mask;mask >>= 1){
		if((mask & *en)){
			if(!(mask & *b.en)){
				return 1;
			} else {
				return 0;
			}
		} else if(mask & *b.en){
			return -1;
		}
	}
}

bool polynom::deg_less(polynom & b){
	return this -> cmp_degree(b) == -1;
}

bool polynom::deg_great(polynom & b){
	return this -> cmp_degree(b) == 1;
}

bool polynom::deg_eq(polynom & b){
	return this -> cmp_degree(b) == 0;
}

bool polynom::operator==(polynom & b){
	size_t size_a = en - bn + 1;
	size_t size_b = b.en - b.bn + 1;
	if(size_a != size_b){
		return false;
	}
	base * p1, * p2;
	for(p1 = bn, p2 = b.bn; p1 <= en && p2 <= b.en; ++p1, ++p2){
		if(*p1 != *p2){
			return false;
		}
	}
	if(p1 <= bn || p2 <= b.en){
		return false;
	}
	return true;
}

bool polynom::operator!=(polynom & b){
	return !(*this == b);
}

istream & operator>>(istream & in, polynom & p){
	std::string s;
	in >> s;
	size_t pol_bits = s.size();
	if(!s.size()){
		if(p.ba){
			p.en = p.bn = p.ba;
			*p.bn = 0;
			return in;
		}
	}
	for(;pol_bits; pol_bits--){
		if(s[pol_bits - 1] == '1'){
			break;
		}
	}
	if(pol_bits == 0){
		if(p.ba){
			p.en = p.bn = p.ba;
			*p.bn = 0;
			return in;
		} else {
			p.ba = new base[1];
			p.ea = p.en = p.bn = p.ba;
			return in;
		}
	}
	s = s.substr(0, pol_bits);
	size_t pol_size = (pol_bits - 1) / BBITS + 1;
	if(!p.ba){
		p.ba = new base[pol_size];
		p.ea = p.ba + pol_size - 1;
		for(base * tmp = p.ba;tmp <= p.ea; ++tmp){
			*tmp = 0;
		}
		p.ea = p.bn = p.en = p.ba;
	}	else {
		polynom::resize(p, pol_size);
		p.clear();
	}
	
	size_t new_ind;
	base mask;
	for(size_t i = 0, mask = 1;i < BBITS;++i, mask <<= 1){
		for(size_t j = 0;j < pol_size; ++j){
			new_ind = j * BBITS + i;
			if(new_ind >= pol_bits){
				break;
			}
			if(s[new_ind] == '1'){
				p.bn[j] |= mask;
			}
		}
	}
	p.en = p.bn + pol_size - 1;
	return in;
}

void polynom::clear(){
	for(base * tmp = ba; tmp <= ea; ++tmp){
		*tmp = 0;
	}
	bn = en = ba;
}


ostream & operator<<(ostream & out, const polynom & p){
	if(p.en == p.bn && *p.bn == 0){
		out << '0';
		return out;
	}
	for(base * tmp = p.bn; tmp < p.en; ++tmp){
		for(base mask = 1; mask; mask <<= 1){
			if(mask & *tmp){
				out << '1';
			} else {
				out << '0';
			}
		}
	}
	base bb = *p.en;
	for(;bb;bb >>= 1){
		if(bb & 1){
			out << '1';
		} else {
			out << '0';
		}
	}
	return out;
}

polynom polynom::operator+(polynom & b){
	size_t size_a = en - bn + 1;
	size_t size_b = b.en - b.bn + 1;
	base * max_b, *max_e, *min_b, *min_e;
	size_t size_max;
	if(size_a > size_b){
		max_b = bn;
		max_e = en;
		min_b = b.bn;
		min_e = b.en;
		size_max = size_a;
	} else {
		max_b = b.bn;
		max_e = b.en;
		min_b = bn;
		min_e = en;
		size_max = size_b;
	}
	polynom res(size_max, 0);
	base * res_tmp = res.ba;
	base * tmp1 = max_b, * tmp2 = min_b;
	for(; tmp2 <= min_e; ++tmp1, ++tmp2, ++res_tmp){
		*res_tmp = *tmp1 ^ *tmp2;
	}
	for(;tmp1 <= max_e; ++tmp1, ++res_tmp){
		*res_tmp = *tmp1;
	}
	res_tmp -= 1;
	for(;*res_tmp == 0 && res_tmp > res.bn; --res_tmp);
	res.en = res_tmp;
	return res;
}

polynom polynom::operator-(polynom & b){
	polynom res = *this + b;
	return res;
}

polynom & polynom::operator+=(polynom & b){
	size_t size_a = en - bn + 1;
	size_t size_b = b.en - b.bn + 1;
	base * max_b, *max_e, *min_b, *min_e;
	size_t size_max;
	if(size_a > size_b){
		size_max = size_a;
	} else {
		size_max = size_b;
	}
	polynom::resize(*this, size_max);
	if(size_a > size_b){
		max_b = bn;
		max_e = en;
		min_b = b.bn;
		min_e = b.en;
	} else {
		max_b = b.bn;
		max_e = b.en;
		min_b = bn;
		min_e = en;
	}
	base * res_tmp = bn;
	base * tmp1 = max_b, * tmp2 = min_b;
	for(; tmp2 <= min_e; ++tmp1, ++tmp2, ++res_tmp){
		*res_tmp = *tmp1 ^ *tmp2;
	}
	for(;tmp1 <= max_e; ++tmp1, ++res_tmp){
		*res_tmp = *tmp1;
	}
	res_tmp -= 1;
	for(;*res_tmp == 0 && res_tmp > bn; --res_tmp);
	en = res_tmp;
	return *this;
}

polynom & polynom::operator-=(polynom & b){
	return *this += b;
}

polynom polynom::operator>>(int num) const {
    if(num < 0 || num > BBITS - 1) {
        std::cout << "shift right" << std::endl << std::flush;
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;
    polynom res(*this);

    base mask = (1 << num) - 1;

    for(int i = en - bn; i >= 0; i--) {
        buffer = res.bn[i] & mask;
        res.bn[i] = (res.bn[i] >> num) | (prev_buf << (BBITS - num));
        prev_buf = buffer;
    }

    if(!(*res.en) && res.en != res.bn) {
        res.en--;
    }

    return res;
}

polynom polynom::operator<<(int num) const {
    if(num < 0 || num > BBITS - 1) {
        std::cout << "shift left" << std::endl << std::flush;
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;
	size_t size_now = en - bn + 1;
	base * p = new base[size_now + 1];
	base * tmp1, *tmp2;
	for(tmp1 = p, tmp2 = bn;tmp2 <= en; ++tmp1, ++tmp2){
		*tmp1 = *tmp2;
	}
	*tmp1 = 0;
    polynom res(tmp1, size_now, size_now + 1);

    base mask = (1 << num) - 1;

    for(tmp1 = res.bn;tmp1 <= res.en; ++tmp1) {
        buffer = (*tmp1 >> (BBITS - num)) & mask;
        *tmp1 = (*tmp1 << num) | prev_buf;
        prev_buf = buffer;
    }

    if(prev_buf) {
        *tmp1 = prev_buf;
    }
	
	for(;*tmp1 == 0 && tmp1 > res.bn;--tmp1);
	res.en = tmp1;

    return res;
}

polynom & polynom::operator>>=(int num) {
    if(num < 0 || num >= BBITS) {
        std::cout << "second shift right" << std::endl << std::flush;
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;

    base mask = (((base)1) << num) - 1;
	base * tmp;
    for(tmp = en; tmp >= bn; --tmp) {
        buffer = *tmp & mask;
       	*tmp = (*tmp >> num) | (prev_buf << (BBITS - num));
        prev_buf = buffer;
    }

    if(!*en && en != bn) {
        en--;
    }

    return *this;
}

polynom & polynom::operator<<=(int num) {
    if(num < 0 || num >= BBITS) {
        std::cout << "second shift left" << std::endl << std::flush;
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;

    base mask = (((base)1) << num) - 1;
	base * tmp;
    for(tmp = bn; tmp <= en; ++tmp) {
        buffer = (*tmp >> (BBITS - num)) & mask;
        *tmp = (*tmp << num) | prev_buf;
        prev_buf = buffer;
    }

    if(prev_buf) {
        polynom::resize(*this, en - bn + 2);
        en++;
        *en = prev_buf;
    }

    return *this;
}


polynom polynom::operator*(polynom & b){
	size_t size_a = en - bn + 1;
	size_t size_b = b.en - b.bn + 1;
	polynom * max, * min;
	size_t size_max;
	size_t size_min;
	if(size_a > size_b){
		max = this;
		min = &b;
		size_min = size_b;
		size_max = size_a;
	} else {
		max = &b;
		min = this;
		size_min = size_a;
		size_max = size_b;
	}
	polynom mm(size_b + 1, 0);
	base * tmp1 = max -> bn, * tmp2 = mm.bn;	
	for(;tmp1 <= max -> en; ++tmp2, ++tmp1){
		*tmp2 = *tmp1;
	}
	mm.en = mm.bn + size_b - 1;

	polynom res(size_a + size_b, 0);
	base mask = 1;
	for(int i = 0; i < BBITS; ++i, mask <<= 1, mm <<= 1){
		//std::cout << mm << std::endl;
		for(tmp1  = min -> bn; tmp1 <= min -> en; ++tmp1){
			if(*tmp1 & mask){
				for(base * p1 = res.bn + (tmp1 - min -> bn), * p2 = mm.bn; p2 <= mm.en; ++p1, ++p2){
					*p1 ^= *p2;
				}
			}
		}
	}

	res.en = res.bn + size_a + size_b - 1;
	for(;*res.en == 0 && res.en > res.bn; --res.en);
	return res;
}

polynom & polynom::operator*=(polynom & b){
	*this = *this * b;
	return *this;
}

void polynom::div_mod(polynom & a, polynom & b, polynom * q, polynom * r){
	if(a.get_deg() < b.get_deg()){
		if(q){
			q -> en = q -> bn;
			*q -> bn = 0;
		}

		if(r){
			size_t size_r = r -> ea - r -> ba + 1;
			size_t size_a = a.en - a.bn + 1;
			if(size_r < size_a){
				polynom::resize(*r, size_a);
			}

			for(base * tmp1 = a.bn, * tmp2 = r -> ba;tmp1 <= a.en; ++tmp1, ++tmp2){
				*tmp2 = *tmp1;
			}
			r -> bn = r -> ba;
			r-> en = r -> ba + size_a - 1;
		}
		return;
	}

	if(q){
		q -> clear();
	}

	if(r){
		r -> clear();
	}

	size_t size_a = a.en - a.bn + 1;
	size_t size_b = b.en - b.bn + 1;

	polynom bb(size_b + 1, 0);
	polynom aa(a);
	polynom res(size_b, 0);


	for(base * tmp1 = bb.bn, * tmp2 = b.bn; tmp2 <= b.en; ++tmp1, ++tmp2){
		*tmp1 = *tmp2;
	}
	bb.en = bb.bn + size_b - 1;	
	int now_shift = 0, n, deg_b, deg_aa, deg_b_r;
	base mask, deg_now, count, j, * now_q;

	for(mask = ((base)1) << (BBITS - 1), count = BBITS - 1;!(mask & *b.en) && mask; mask >>= 1, --count);

	deg_b = count + BBITS * (b.en - b.bn);
	deg_b_r = count;
	deg_aa = aa.get_deg();
	for(;;){
		mask = ((base)1) << (BBITS - 1);
		count = BBITS - 1;
		for(;!(mask & *aa.en) && mask;mask >>= 1, --count);
		n = now_shift + (int32_t)deg_b_r;
		if(count < deg_b_r){
			count += BBITS;
		}
		if(count > n){
			bb <<= (count - n);
			now_shift += (count - n);
		} else if(count < n){
			bb >>= (n - count);
			now_shift -= (n - count);
		}
	//	std::cout << bb << std::endl << now_shift << std::endl;
		for(base * p1 = aa.en, * p2 = bb.en; p2 >= bb.bn; --p1, --p2){
			*p1 ^= *p2;
		}

		if(q){
			now_q = q -> bn + ((deg_aa - deg_b)/ BBITS);
			*now_q |= (((base)1) << now_shift);
		}

		for(;*aa.en == 0 && aa.en > aa.bn; --aa.en);
		if((deg_aa = aa.get_deg()) < deg_b){
			break;
		}
	}

	if(q){
		q -> en = q -> bn + (a.get_deg() - deg_b)/BBITS;
	}

	if(r){
		size_t size_r = r -> ea - r -> ba + 1;
		size_t size_aa = aa.en - aa.bn + 1;
		if(size_r < size_aa){
			polynom::resize(*r, size_aa);
		}
		for(base * tmp1 = aa.bn, * tmp2 = r -> ba;tmp1 <= aa.en; ++tmp1, ++tmp2){
			*tmp2 = *tmp1;
		}
		r -> bn = r -> ba;
		r -> en = r -> ba + size_aa - 1;
	}
}

int polynom::get_deg(){
	base mask;
	int count;
	mask = ((base)1) << (BBITS - 1);
	count = BBITS - 1;
	for(;!(*en & mask) && mask; mask >>= 1, --count);
	if(!mask){
		return -1;
	}
	return count + (en - bn) * BBITS;
}

polynom polynom::operator/(polynom & b){
	polynom res((this -> get_deg() - b.get_deg())/BBITS + 1, 0);
	polynom::div_mod(*this, b, &res, nullptr);
	return res;
}

polynom polynom::operator%(polynom & b){
	polynom res(b.en - b.bn + 1, 0);
	polynom::div_mod(*this, b, nullptr, &res);
	return res;
}

polynom polynom::pow(BigInt & deg, polynom & mod){
	polynom res(0), z(*this);
	base b;
	for(base * tmp = deg.bn;tmp <= deg.en; ++tmp){
		b = *tmp;
		if(b){
			for(int i = 0;i < BBITS && (b || deg.en > tmp); b >>=1, ++i){
				if(b & 1){
					res *= z;
					res = res % mod;
				}
				z *= z;
				z = z % mod;
			}
		} else {
			for(int i = 0;i < BBITS; ++i){
				z *= z;
				z = z % mod;
			}
		}
	}
	return res;
}

polynom polynom::pow(base deg, polynom & mod){
	polynom res(0), z(*this);
	for(int i = 0;i < BBITS; deg >>=1, ++i){
		if(deg & 1){
			res *= z;
			res = res % mod;
		}
		z *= z;
		z = z % mod;
	}
	return res;
}


polynom polynom::gcd(polynom & m){
	polynom a, b, r;
	
	if(this ->get_deg() > m.get_deg()){
		a = *this;
		b = m;
	} else {
		b = *this;
		a = m;
	}
	r = a % b;
	for(;!(r.en == r.bn && *r.bn == 0);){
		a = b;
		b = r;
		r = a % b;
	}
	return b;
}

bool polynom::is_irreductible_1(){
	int n = this -> get_deg() >> 1;
	std::cout << *this << std::endl;
	polynom d, u(1);
	for(int i = 0;i < n; ++i){
		u *= u;
		std::cout << u << std::endl;
		u = u % *this;
		std::cout << u << std::endl;
		*u.bn ^= 0x2;
		d = u.gcd(*this);
		std::cout << u << std::endl << *this << std::endl << d << std::endl;
		if(!(d.en == d.bn && *d.bn == 1)){
			return false;
		}
		*u.bn ^= 0x2;
	}
	return true;
}

bool polynom::is_irreductible_2(){
	base deg = this -> get_deg();
	if(deg < 2){
		return true;
	}
	size_t size_p = en - bn + 1;
	polynom u(size_p,0), d;
	*u.bn = 0x2;
	for(base i = 0; i < deg; ++i){
		u *= u;
		u = u % *this;
	}
	if(!(u.bn == u.en && *u.bn == 0x2)){
		return false;
	}
	BigInt n(deg);
	BigInt_D div = n.factor();

	base now_deg;
	
	for(auto it = div.begin(); it != div.end(); ++it){
		now_deg = deg / *((*it) -> num -> bn);
		u.clear();
		*u.bn = 0x2;
		for(base i = 0; i < now_deg; ++i){
			u *= u;
			u = u % *this;
		}
		*u.bn ^= 0x2;
		d = u.gcd(*this);
		if(!(d.bn == d.en && *d.bn == 1)){
			return false;
		}
	}

	return true;
}
