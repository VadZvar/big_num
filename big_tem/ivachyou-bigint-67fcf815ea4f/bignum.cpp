#include "bignum.h"

BigInt::BigInt(){
	ba = nullptr;
}

BigInt::BigInt(base b) {
    ba = new base[1];
    if(!ba) {
        throw ALLOC_ERR;
    }
    bn = ea = en = ba;
    bn[0] = b;
}

BigInt::BigInt(int b) {
    ba = new base[1];
    if(!ba) {
        throw ALLOC_ERR;
    }
    bn = ea = en = ba;
    bn[0] = (base)b;
}

BigInt::BigInt(size_t size, base b) {
    ba = new base[size];
    if(!ba) {
        throw ALLOC_ERR;
    }
    ea = ba + size - 1;
    bn = ba;
    for(int i = 0; i < size; i++) {
        bn[i] = b;
    }
    if(b) {
        en = ea;
    } else {
        en = bn;
    }
}

BigInt::BigInt(base * b, size_t size, size_t real_size) {
    ba = bn = b;
    en = b + size - 1;
    ea = b + real_size - 1;
	for(;en > bn && !(*en); --en);
}

BigInt * BigInt::get_degree_of_two(size_t num_base, size_t num_bit){
	if(num_bit < 0 || num_bit > BBITS){
		throw "bad num_bit";
	}
	base * bb = new base[num_base], * now;
	now = bb;
	for(int i = 0; i < num_base - 1; ++i, ++now){
		*now = (base)0;
	}
	*now = 1 << num_bit;
	return new BigInt(bb, num_base, num_base);
}

BigInt::BigInt(const BigInt & b) {
    int size = b.en - b.bn + 1;
    ba = new base[size];
    if(!ba) {
        throw ALLOC_ERR;
    }
    ea = ba + size - 1;
    en = ea;
    bn = ba;
    for(int i = 0; i < size; i++) {
        bn[i] = b.bn[i];
    }
}

BigInt & BigInt::operator=(const BigInt & tmp) {
    if(this == &tmp) {
        return *this;
    }
    size_t size = tmp.en - tmp.bn + 1;
    if(ba) delete[] ba;
    ba = new base[size];
    if(!ba) {
        throw ALLOC_ERR;
    }
    bn = ba;
    ea = ba + size - 1;
    en = ea;
    for(int i = 0; i < size; i++) {
        bn[i] = tmp.bn[i];
    }
    return *this;
}

BigInt::~BigInt() {
	if(ba) {
		delete [] ba;
	}
}

BigInt BigInt::operator +(const BigInt & b) const {
    size_t size_max;
    size_t size_min;
    base * max, * min;
    int i;

    if(en - bn >= b.en - b.bn) {
        size_max = en - bn + 1;
        size_min = b.en - b.bn + 1;
        max = bn;
        min = b.bn;
    } else {
        size_max = b.en - b.bn + 1;
        size_min = en - bn + 1;
        max = b.bn;
        min = bn;
    }

    BigInt res(size_max + 1, 0);
    dbase buffer = 0;

    for(i = 0; i < size_min; i++) {
        buffer += (dbase)max[i] + (dbase)min[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }

    for(; i < size_max; i++) {
        buffer += (dbase)max[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }

    if(buffer) {
        res.bn[size_max] = (base)buffer;
        res.en = res.bn + size_max;
    } else res.en = res.bn + size_max - 1;

    return res;
}

void BigInt::plus(const BigInt & b, BigInt  & res) const{
    size_t size_max;
    size_t size_min;
    base * max, * min;
    int i;

    if(en - bn >= b.en - b.bn) {
        size_max = en - bn + 1;
        size_min = b.en - b.bn + 1;
        max = bn;
        min = b.bn;
    } else {
        size_max = b.en - b.bn + 1;
        size_min = en - bn + 1;
        max = b.bn;
        min = bn;
    }

    dbase buffer = 0;

    for(i = 0; i < size_min; i++) {
        buffer += (dbase)max[i] + (dbase)min[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }

    for(; i < size_max; i++) {
        buffer += (dbase)max[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }

    if(buffer) {
        res.bn[size_max] = (base)buffer;
        res.en = res.bn + size_max;
    } else res.en = res.bn + size_max - 1;

    return;

}

BigInt & BigInt::operator+=(const BigInt & b) {
   resize(*this, en - bn + 2);
   this -> plus(b, *this);
   return *this;
}

BigInt BigInt::operator-(const BigInt & b) const {
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if(size_b > size_a) {
		throw "<0";
    }

    int i;
    BigInt res(size_a, 0);
    dbase buffer = 0;

    for(i = 0; i < size_b; i++) {
        buffer = (dbase)bn[i] - (dbase)b.bn[i] - buffer;
        res.bn[i] = buffer;
        buffer >>= BBITS;
        buffer = (buffer)?1:0;
    }
    for(; i < size_a && buffer; i++) {
        buffer = (dbase)bn[i] - buffer;
        res.bn[i] = buffer;
        buffer >>= BBITS;
        buffer = (buffer)?1:0;
    }
    for(; i < size_a; i++) {
        res.bn[i] = bn[i];
    }

    if(buffer > 0) {
        throw "<0";
    }

    if(i == size_a && !res.bn[size_a - 1] && size_a != 1) {
        res.en = res.bn + size_a - 2;
		for(;*res.en == 0 && res.en > res.bn; res.en--);
    } else res.en = res.bn + size_a - 1;

    return res;
}

void BigInt::minus(const BigInt & b, BigInt & res){
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if(size_b > size_a) {
        throw "<0";
    }
	BigInt res(*this);

    int i;
    dbase buffer = 0;

    for(i = 0; i < size_b; i++) {
        buffer = (dbase)bn[i] - (dbase)b.bn[i] - buffer;
        res.bn[i] = buffer;
        buffer >>= BBITS;
        buffer = (buffer)?1:0;
    }
    for(; i < size_a && buffer; i++) {
        buffer = (dbase)bn[i] - buffer;
        res.bn[i] = buffer;
        buffer >>= BBITS;
        buffer = (buffer)?1:0;
    }
    for(; i < size_a; i++) {
        res.bn[i] = bn[i];
    }

    if(buffer > 0) {
		throw "<0";
    }

    if(i == size_a && !res.bn[size_a - 1] && size_a != 1) {
        res.en = res.bn + size_a - 2;
		for(;*res.en == 0 && res.en > res.bn; res.en--);
    } else res.en = res.bn + size_a - 1;

    return;

}

BigInt & BigInt::operator-=(const BigInt & b) {
	this -> minus(b, *this);
	return *this;
}

BigInt BigInt::operator*(BigInt & b){
    return this -> fmul2(b);
}

BigInt BigInt::usual_mul(BigInt & b){
	dbase buffer = 0;
    int i, j;
    size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;

    if(size_a == 1 && *bn == 0 || size_b == 1 && *b.bn == 0) {
        return BigInt(0);
    }

    BigInt res(size_a + size_b, 0);

    for(i = 0; i < size_b; i++) {
        for(j = 0; j < size_a; j++) {
            buffer += (dbase)(bn[j]) * (dbase)(b.bn[i]) + (dbase)(res.bn[i + j]);
            res.bn[i + j] = (base)buffer;
            buffer >>= BBITS;
        }
        for(; buffer && j < size_a + size_b; j++) {
            buffer += res.bn[i + j];
            res.bn[i + j] = (base)buffer;
            buffer >>= BBITS;
        }
    }
    if(res.bn[size_a + size_b - 1]) {
        res.en = res.bn + size_a + size_b - 1;
    } else {
        res.en = res.bn + size_a + size_b - 2;
    }

    return res;
}

void BigInt::usual_mul_mem(BigInt & b, BigInt & res){
	dbase buffer = 0;
    int i, j;
    size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;

    if(size_a == 1 && *bn == 0 || size_b == 1 && *b.bn == 0) {
        res.en = res.bn;
		*(res.bn) = 0;
		return;
    }

    for(i = 0; i < size_b; i++) {
        for(j = 0; j < size_a; j++) {
            buffer += (dbase)(bn[j]) * (dbase)(b.bn[i]) + (dbase)(res.bn[i + j]);
            res.bn[i + j] = (base)buffer;
            buffer >>= BBITS;
        }
        for(; buffer && j < size_a + size_b; j++) {
            buffer += res.bn[i + j];
            res.bn[i + j] = (base)buffer;
            buffer >>= BBITS;
        }
    }
    if(res.bn[size_a + size_b - 1]) {
        res.en = res.bn + size_a + size_b - 1;
    } else {
        res.en = res.bn + size_a + size_b - 2;
    }


    return;
}

BigInt & BigInt::operator *=(BigInt &b) {
    return (*this = this -> fmul(b));
}

BigInt BigInt::operator*(base b){
    dbase buffer = 0;
    int i;
    size_t size_a = en - bn + 1;
    if(b == 0 || size_a == 1 && *bn == 0) {
        return BigInt(0);
    }

    BigInt res(size_a + 1, 0);
    for(i = 0; i < size_a; i++) {
        buffer += (dbase)bn[i] * b;
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }

    if(buffer) {
        res.bn[size_a] = (base)buffer;
        res.en = res.bn + size_a;
    } else {
        res.en = res.bn + size_a - 1;
    }

    return res;
}

BigInt & BigInt::operator*=(base b) {
    dbase buffer = 0;
    int i;
    size_t size_a = en - bn + 1;

    if(b == 0 || size_a == 1 && *bn == 0) {
        en = bn;
        *bn = 0;
        return *this;
    }

    BigInt::resize(*this, size_a + 1);
    for(i = 0; i < size_a; i++) {
        buffer += (dbase)bn[i] * b;
        bn[i] = buffer;
        buffer >>= BBITS;
    }

    if(buffer) {
        bn[size_a] = buffer;
        en = bn + size_a;
    } else {
        en = bn + size_a - 1;
    }

    return *this;
}

std::istream & operator>>(std::istream & in, BigInt & b) {
    std::string str;
    int i;

    in >> str;
    BigInt::resize(b, str.size()/4 + 1);
    for(i = 0; i < str.size()/4 + 1; i++) {
        b.bn[i] = 0;
    }

    b.en = b.bn;

    for(int i = 0; i < str.size(); i++) {
        if(str[i] < '0' || str[i] > '9') {
            throw "bad string";
        }
        b *= (base)10;
        b += (base)(str[i] - '0');
    }

    return in;
}

BigInt::BigInt(const std::string & str) {
    int i;
    en = bn = ba = new base[str.size()/4 + 1];
    ea = ba + str.size()/4;
    *bn = 0;

    for(int i = 0; i < str.size(); i++) {
        if(str[i] < '0' || str[i] > '9') {
            throw "bad string";
        }
        *this *= (base)10;
        *this += (base)(str[i] - '0');
    }
}

std::ostream & operator<<(std::ostream & out, const BigInt & b) {
    std::stack<char> st;
    BigInt copy(b);
    BigInt lolo(0);
    base num;

    if(copy == lolo) {
        out << "0";
        return out;
    }
    for(; copy.en - copy.bn > 0 || *copy.bn;) {
        BigInt::div_mod(copy, (base)10, &copy, &num);
        st.push((char)(num + '0'));
    }

    for(; !st.empty(); st.pop()) {
        out << st.top();
    }

    return out;
}

void BigInt::resize(BigInt & b, size_t new_size) {
    int i;
    size_t size_b = b.en - b.bn + 1;
    if(new_size <= 0) {
		throw "bad size";
    }
    if(new_size > size_b) {
        if(new_size > b.ea - b.ba + 1) {
            base * a = new base[new_size];
            if(!a) {
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
}

int BigInt::cmp(const BigInt & a, const BigInt & b) {
    size_t size_a = a.en - a.bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if(size_a > size_b) {
        return 1;
    } else if(size_a < size_b) {
        return -1;
    }
    for(int i = size_a - 1; i >= 0; i--) {
        if(a.bn[i] > b.bn[i]) {
            return 1;
        } else if(a.bn[i] < b.bn[i]) {
            return -1;
        }
    }
    return 0;
}

DEF_OPT(BigInt,==);
DEF_OPT(BigInt,!=);
DEF_OPT(BigInt,>);
DEF_OPT(BigInt,<);

BigInt BigInt::operator>>(int num) const {
    if(num < 0 || num > BBITS - 1) {
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;
    BigInt res(*this);

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

BigInt BigInt::operator<<(int num) const {
    if(num < 0 || num > BBITS - 1) {
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;
    BigInt res(*this);

    base mask = (1 << num) - 1;

    for(int i = 0; i < en - bn + 1; i++) {
        buffer = (res.bn[i] >> (BBITS - num)) & mask;
        res.bn[i] = (res.bn[i] << num) | prev_buf;
        prev_buf = buffer;
    }

    if(prev_buf) {
        BigInt::resize(res, en - bn + 2);
        res.en++;
        *res.en = prev_buf;
    }

    return res;
}

BigInt & BigInt::operator>>=(int num) {
    if(num < 0 || num > BBITS - 1) {
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;

    base mask = (1 << num) - 1;

    for(int i = en - bn; i >= 0; i--) {
        buffer = bn[i] & mask;
        bn[i] = (bn[i] >> num) | (prev_buf << (BBITS - num));
        prev_buf = buffer;
    }

    if(!*en && en != bn) {
        en--;
    }

    return *this;
}

BigInt & BigInt::operator<<=(int num) {
    if(num < 0 || num > BBITS - 1) {
		throw "so big";
    }

    base buffer = 0, prev_buf = 0;

    base mask = (1 << num) - 1;

    for(int i = 0; i < en - bn + 1; i++) {
        buffer = (bn[i] >> (BBITS - num)) & mask;
        bn[i] = (bn[i] << num) | prev_buf;
        prev_buf = buffer;
    }

    if(prev_buf) {
        BigInt::resize(*this, en - bn + 2);
        en++;
        *en = prev_buf;
    }

    return *this;
}

void BigInt::div_mod(const BigInt & a, base b, BigInt * q, base * r) {
    if(b == 0) {
		throw "division on zero";
    }
    dbase buffer = 0, tmp;
    size_t size_a = a.en - a.bn + 1;

    if(size_a == 1) {
        base old = (*a.bn);
        if(*(a.bn)) {
            if(q) {
                q -> en = q -> bn;
                *(q -> bn) = old / b;
            }
            if(r) {
                *r = old % b;
            }
            return;
        } else {
            if(q) {
                q -> en = q -> bn;
                *(q -> bn) = 0;
            }
            if(r) {
                *r = 0;
            }
            return;
        }
    }

    if(q) BigInt::resize(*q, size_a);

    for(int j = size_a - 1; j >= 0; j--) {
        buffer = (buffer << BBITS) | a.bn[j];
        tmp = buffer / (dbase)b;
        if(q) q -> bn[j] = (base)tmp;
        buffer = buffer - (dbase)b * tmp;
    }

    if(r) {
        *r = (base) buffer;
    }

    if(q) {
        if((q -> bn)[size_a - 1] != 0) {
            q -> en = q -> bn + size_a - 1;
        } else q -> en = q -> bn + size_a - 2;
    }
}

BigInt BigInt::operator/(base b)const {
    BigInt res(en - bn + 1, 0);
    div_mod(*this, b, &res, nullptr);
    return res;
}

base BigInt::operator%(base b) const {
    base res;
    div_mod(*this, b, nullptr, &res);
    return res;
}

void BigInt::div_mod(const BigInt & a, BigInt & b, BigInt * q, BigInt * r) {
    if(b == 0) {
		throw "division on zero";
    }
    int i, c1 = BBITS - 1;
    dbase c2 = (dbase)1 << BBITS;
    size_t size_a = a.en - a.bn + 1, size_b = b.en - b.bn + 1;

    if(size_b == 1) {
        div_mod(a,*(b.bn), q, (r)?r -> bn:nullptr);
        if(r) {
            r -> en = r -> bn;
        }
        return;
    } else {
        if(a < b) {
            if(r) {
                for(i = 0; i < size_a; i++) {
                    r -> bn[i] = a.bn[i];
                }
                r -> en = r -> bn + size_a - 1;
            }

            if(q) {
                *(q -> bn) = 0;
                q -> en = q -> bn;
            }
            return;
        }
        base * q1;

        int count = 0;
        base * ub = new base[size_a + 5];
        if(!ub) {
            throw ALLOC_ERR;
        }
        ub[size_a + 1] = ub[size_a] = 0;
        for(i = 0; i < size_a; i++) {
            ub[i] = a.bn[i];
        }
        BigInt u(ub, size_a, size_a + 2);
		
        base mask = (base)(1 << c1);

        for(i = c1; i >= 0; mask >>= 1, i--) {
            if(*(b.en) & mask) {
                count = c1 - i;
                i = -1;
            }
        }

        b <<= count;
        u <<= count;

        dbase qhat, rhat;

        size_a = u.en - u.bn + 1;
        size_b = b.en - b.bn + 1;
        if(q) {
            q1 = q -> en = q -> bn + size_a - size_b;
        }
        if(size_a > size_b) u.bn = u.en - size_b + 1;

        for(int j = size_a; j >= size_b; j--) {
            qhat = ((((dbase)u.bn[size_b]) << BBITS) +  u.bn[size_b - 1]);
            rhat = qhat % (*b.en);
            qhat /= (*b.en);
            for(; rhat < c2 && qhat * (*(b.en - 1)) > (rhat << BBITS) + u.bn[size_b - 2];) {
                qhat--;
                rhat += *(b.en);
            }
            if(b * (base)qhat > u) {
                qhat--;
                u += b;
            }
            u.en = u.bn + size_b;
            u -= b * qhat;
            u.bn--;
            if(q) {
                *q1 = qhat;
                q1--;
            }
        }
        b >>= count;
        if(r) {
            u.bn++;
            u >>= count;
            for(i = 0; i < u.en - u.bn + 1; i++) {
                r -> bn[i] = u.bn[i];
            }
            r -> en = r -> bn + (u.en - u.bn);
			if(!(b > *r)){
				*r -= b;
			}
        }
        if(q) {
            q1++;
            q -> bn = q1;
            for(; !*(q -> en); q -> en--);
        }
    }
}

BigInt BigInt::operator/(BigInt & b) const {
    if(en - bn >= b.en - b.bn) {
        BigInt res((en - bn) - (b.en - b.bn) + 2, 0);
        div_mod(*this, b, &res, nullptr);
        return res;
    } else {
        return BigInt(0);
    }
}

BigInt BigInt::operator%(BigInt & b) const {
    if(en - bn >= b.en - b.bn) {
        BigInt res(b.en - b.bn + 1, 0);
        div_mod(*this, b, nullptr, &res);
        return res;
    } else {
        return BigInt(*this);
    }
}

BigInt BigInt::sqr(){
	return *this * *this;
}

BigInt & BigInt::barret(BigInt & mod, BigInt & num){
	size_t k = mod.en - mod.bn + 1;

	if(mod == 0){
		throw "bad mod";
	}
	if(*this < mod){
		return *this;
	}
	base * end = en, * now, * now_r;
	if(en - bn > k){
		en = bn + k;
	}
	for(now = bn + k + 1;now <= ea; *now = 0, ++now);
	for(;*en == 0 && bn < en; --en);
	BigInt rem(*this);
	base * tmp = new base[k + 2];
	for(now = tmp;now < tmp + k + 1; *now = 0, ++now);
	for(now = tmp, now_r = bn; now < tmp + k + 1 && now_r <= en; ++now, ++now_r){
		*now = *now_r;
	}

	BigInt r1(tmp, k + 1, k + 2);
	en = end;

	bn += k - 1;
	*this = *this * num;
	bn += k + 1;
	BigInt nn(rem);
	rem = rem / mod;
	*this = *this * mod;
	if(en - bn > k ){
		en = bn + k;
	}
	for(now = bn + k + 1;now <= ea; *now = 0, ++now);
	for(;*en == 0 && bn < en; --en);
	if(!(*this > r1)){
		r1 -= *this;
	} else {
		size_t size_this = en - bn;
		base mask = ((base)1) << (BBITS - 1);
		base shft = BBITS - 1;
		for(;!(mask & *en); mask>>=1, --shft); 
		BigInt::resize(r1, k + 2);
		size_this = (size_this > k)?(k):(size_this);
		r1.en = r1.bn + size_this;
		*(r1.en) += 1 << shft;
		r1 -= *this;
	}
	for(;!(r1 < mod);){
		r1 -= mod;
	}
	delete [] ba;
	ba = r1.ba;
	bn = r1.bn;
	ea = r1.ea;
	en = r1.en;
	r1.ba = nullptr;
	return *this;

}

BigInt BigInt::pow(const BigInt & degree, BigInt & mod){
	BigInt res(1), z(*this);
	base * end, * now;
	base tmp;
	size_t size_mod = mod.en - mod.bn + 1;
	int i;
	BigInt * d2 = BigInt::get_degree_of_two((size_mod << 1) + 1, 0), num;
	num = (*d2) / mod;
	delete d2;
	z = z % mod;
	for(now = degree.bn; now <= degree.en; ++now){
		tmp = *now;
		if(tmp){
			for(i = 0; i < sizeof(tmp) * 8; ++i, tmp >>= 1){
				//z = z % mod;
				if(tmp & 1){
					res = res * z;
					res = res% mod;//.barret(mod, num); //% mod;
				}
				z = z.sqr();
				z = z % mod; //.barret(mod, num);		
			}	
		} else {
			for(i = 0; i < sizeof(tmp) * 8; ++i){
				z = z.sqr();
				z= z % mod;//barret(mod, num);
			}	
		}
	}

	return res;
}

BigInt BigInt::fmul2(BigInt & b){
	size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;
	BigInt res(size_a + size_b + 1, 0);
	
	this -> fmul_car(b, res);
	return res;
}

void BigInt::fmul_car(BigInt & b, BigInt & res) {
	size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;
	if(CAR_BORDER < 3){
		throw "bag CAR_BORDER";
	}
	
	if(size_a > CAR_BORDER && size_b > CAR_BORDER){
		size_t mask = (size_a > size_b)?(size_a>>1):(size_b>>1);
		if(mask < size_a && mask < size_b){
			BigInt a1(bn + mask, size_a - mask, size_a - mask),\
					a2(bn, mask, mask),\
					b1(b.bn + mask, size_b - mask, size_b - mask),\
					b2(b.bn, mask, mask);
			
			size_t size_a1 = a1.en - a1.bn + 1, \
					size_a2 = a2.en - a2.bn + 1, \
					size_b1 = b1.en - b1.bn + 1, \
					size_b2 = b2.en - b2.bn + 1;
			size_t mm1 = (size_a1 > size_b1)?(size_a1 + 1):(size_b1 + 1),\
					mm2 = (size_a2 > size_b2)?(size_a2 + 1):(size_b2 + 1);
			size_t num = (mask << 1) + 4 + size_a1 + size_a2 + size_b1 + size_b2 + \
							 + mm1 + mm2;
							
			base * tmp = new base[num];
			base * now;	
			for(now = tmp; now < tmp + num; ++now){
				*now = 0;
			}
			now = tmp;
			BigInt r1(now, 1, mask + 1);
			now += mask + 1;
			BigInt r2(now, 1, mask + 1);
			now += mask + 1;
			BigInt r3(now, 1, size_a1 + size_b1 + 1);
			now += size_a1 + size_b1;
			BigInt r4(now, 1, size_a2 + size_b2 + 1);
			now += size_a2 + size_b2;
			BigInt r5(now, 1, mm1 + mm2 + 1);


			a1.plus(a2, r1);
			b1.plus(b2, r2);
			
			a2.fmul_car(b2, r4);
			a1.fmul_car(b1, r3);
			r1.fmul_car(r2, r5);
			
			r5 -= r3;
			r5 -= r4;
			
			base * now_res, * now_r;

			for(now_res = res.bn, now_r = r4.bn;now_r <= r4.en; ++now_r, ++now_res){
				*now_res = *now_r;
			}

			now_res = res.bn + mask;
			
			dbase buffer = 0;
			
			for(now_r = r5.bn; now_r <= r5.en; ++now_r, ++now_res){
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(;buffer; ++now_res){
				buffer += (dbase) *now_res;
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			now_res = res.bn + (mask << 1);

			for(now_r = r3.bn; now_r <= r3.en; ++now_r, ++now_res){
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(;buffer; ++now_res){
				buffer += (dbase)*now_res;
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(now_res = res.bn + size_a + size_b - 1; !(*now_res); --now_res);
			res.en = now_res;

			a1.ba = a2.ba = b1.ba = b2.ba = r1.ba = r2.ba = r3.ba = r4.ba = r5.ba = nullptr;
			delete [] tmp;
		} else {
			BigInt max1, max2, min;
			size_t size_min, size_max1, size_max2, num; 
			if(size_a > size_b){
				max1.ba = max1.bn = bn + mask;
				max1.ea = max1.en = en;
				max2.ba = max2.bn = bn;
				max2.ea = max2.en = bn + mask - 1;
				for(;max2.bn < max2.en && !(*max2.en);--(max2.en));
				min.ba = min.bn = b.bn;
				min.en = min.ea = b.en;
			} else {
				max1.ba = max1.bn = b.bn + mask;
				max1.ea = max1.en = b.en;
				max2.ba = max2.bn = b.bn;
				max2.ea = max2.en = b.bn + mask - 1;
				min.ba = min.bn = bn;
				min.en = min.ea = en;
			}
			size_min = min.en - min.bn + 1;
			size_max1 = max1.en - max1.bn + 1;
			size_max2 = max2.en - max2.bn + 1;
			num = (size_min << 2) + size_max1 + size_max2;

			base * tmp = new base[num], * now;

			for(now = tmp; now < tmp + num; ++now){
				*now = 0;
			}
			
			now = tmp;
			BigInt r1(now, 1, size_max1 + size_min);
			now += size_min + size_max1;
			BigInt r2(now, 1, size_max2 + size_min);

			max1.fmul_car(min, r1);
			max2.fmul_car(min, r2);
			
			base * now_res = res.bn, * now_r;
			
			dbase buffer = 0;

			for(now_r = r2.bn; now_r <= r2.en; ++now_r, ++now_res){
				*now_res = *now_r;
			}

			now_res = res.bn + mask;

			for(now_r = r1.bn; now_r <= r1.en; ++now_r, ++now_res){
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(;buffer; ++now_res){
				buffer += (dbase)(*now_res);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(now_res = res.bn + size_a + size_b - 1; !(*now_res); --now_res);
			res.en = now_res;

			max1.ba = max2.ba = min.ba = r1.ba = r2.ba = nullptr;
			delete [] tmp;
		}
	} else {
		try{
		this -> usual_mul_mem(b, res);
		} catch(const char * e){
			throw "huynya";
		}
	}
	return;
}


BigInt BigInt::fmul(BigInt & b) {
	size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;
	if(CAR_BORDER < 3){
		throw "bag CAR_BORDER";
	}
	

	BigInt res(size_a + size_b + 1, 0);
	if(size_a > CAR_BORDER && size_b > CAR_BORDER){
		size_t mask = (size_a > size_b)?(size_a>>1):(size_b>>1);
		if(mask < size_a && mask < size_b){
			BigInt a1(bn + mask, size_a - mask, size_a - mask),\
					a2(bn, mask, mask),\
					b1(b.bn + mask, size_b - mask, size_b - mask),\
					b2(b.bn, mask, mask);
			
			//BigInt r1 = a1 + a2, r2 = b1 + b2
			BigInt r3 = a1.fmul(b1);
			BigInt r1(mask + 1, 0), r2(mask + 1, 0);
			a1.plus(a2, r1);
			b1.plus(b2, r2);
			BigInt r4 = a2.fmul(b2);
			r1 = r1.fmul(r2);
			r1 -= r3;
			r1 -= r4;
			base * now_res, * now_r;
			int i = 0;

			for(now_res = res.bn, now_r = r4.bn;now_r <= r4.en; ++now_r, ++i, ++now_res){
				*now_res = *now_r;
			}

			now_res = res.bn + mask;
			
			i = 0;

			dbase buffer = 0;
			
			for(now_r = r1.bn; now_r <= r1.en; ++now_r, ++now_res, ++i){
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(;buffer; ++now_res){
				buffer += (dbase) *now_res;
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			now_res = res.bn + (mask << 1);

			for(now_r = r3.bn; now_r <= r3.en; ++now_r, ++now_res){
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(;buffer; ++now_res){
				buffer += (dbase)*now_res;
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(now_res = res.bn + size_a + size_b - 1; !(*now_res); --now_res);
			res.en = now_res;

			a1.ba = a2.ba = b1.ba = b2.ba = nullptr;
		} else {
			BigInt max1, max2, min;
			if(size_a > size_b){
				max1.ba = max1.bn = bn + mask;
				max1.ea = max1.en = en;
				max2.ba = max2.bn = bn;
				max2.ea = max2.en = bn + mask - 1;
				for(;max2.bn < max2.en && !(*max2.en);--(max2.en));
				min.ba = min.bn = b.bn;
				min.en = min.ea = b.en;
			} else {
				max1.ba = max1.bn = b.bn + mask;
				max1.ea = max1.en = b.en;
				max2.ba = max2.bn = b.bn;
				max2.ea = max2.en = b.bn + mask - 1;
				min.ba = min.bn = bn;
				min.en = min.ea = en;
			}
			
			BigInt r1 = max1.fmul(min), r2 = max2.fmul(min);
			base * now_res = res.bn, * now_r;
			dbase buffer = 0;
			int i = 0;
			for(now_r = r2.bn; now_r <= r2.en; ++i, ++now_r, ++now_res){
				*now_res = *now_r;
			}

			now_res = res.bn + mask;

			for(now_r = r1.bn; now_r <= r1.en; ++now_r, ++now_res){
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(;buffer; ++now_res){
				buffer += (dbase)(*now_res);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for(now_res = res.bn + size_a + size_b - 1; !(*now_res); --now_res);
			res.en = now_res;

			max1.ba = max2.ba = min.ba = nullptr;
		}

		return res;
	
	} else {
		return this -> usual_mul(b);
	}
}

bool BigInt::miller_rabin_primality_test(){
	if(!(*this > 3)){
		if(*bn == 1){
			return false;
		} else return true;
	}
	if(!(*bn & 1)){
		return false;
	}

	BigInt r = *this - 1, b;
	BigInt decn = r;
	size_t size_p = en - bn + 1, sbits, sbase, s;
	base mask, * now;
	for(now = r.bn; now <= r.en; ++now){
		mask = 1;
		for(int j = 0; j < BBITS; mask <<=1, ++j){
			if(mask & *now){
				sbits = j;
				sbase = now - r.bn;
				j = BBITS;
				now = r.en + 1;
			}
		}
	}
	r.bn += sbase;
	
	if(sbits){
		r >>= sbits;
	}
	
	s = BBITS * sbase + sbits;
	
	int i;
	for(i = MR_REL_PARAM; i > 0; --i){
		
		b = BigInt::gen_num_less_than(decn);
		
		while(b < 2){
			b = BigInt::gen_num_less_than(decn);
		}
		
		b = b.pow(r, *this);
		
		if(b != 1 && b != decn){
			for(int j = 0; j < s - 1 && b != decn; ++j){
				b = b.sqr() % *this;
				if(b == 1){
					return false;
				}
			}
			if(b != decn){
				return false;
			}
		}
	}

	return true;
}

bool BigInt::is_prime(){
	return this -> miller_rabin_primality_test();
}

BigInt BigInt::gen_prime(size_t bits){
	BigInt b = BigInt::gen_num_with_bits(bits);
	if(!(*(b.bn) & 1)){
		b += 1;
	}
	for(;!(b.is_prime()); b += 2);
	return b;
}

BigInt BigInt::random(int length) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srand(ts.tv_nsec);
    base * ba, * ea, *en;
    int num_length = length;
    ba = new base[length];
    if(!ba) {
        throw ALLOC_ERR;
    }
    ea = ba + length - 1;
    base * t = ea;
    en = t;
    while(t > ba && !(*t = (base) rand())) {
        t--;
        num_length--;
    }
    for(;t >= ba; --t) {
        *t = (base) rand();
    }
    return BigInt(ba, num_length, length);
}

BigInt BigInt::gen_num_with_bits(size_t num){
	size_t num_base = (num / BBITS) + 1, num_bits = num;
	return BigInt::gen_num_with_bits(num_base, num_bits);
}

BigInt BigInt::gen_num_with_bits(size_t num_base, size_t num_bits){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srand(ts.tv_nsec);
	base * ba, * en, *bn, * t;
	bn = ba = new base[num_base];
	en = ba + num_base - 1;
	if(num_bits >= BBITS){
		num_bits %= BBITS;
	}
	if(num_bits == 0){
		if(num_base > 1){
			--num_base;
			num_bits = BBITS - 1;
		} else {
			throw "strange params";
		}
	}

	base mask = (1 << num_bits) - 1;
	*en = rand() & mask;
	mask = 1 << (num_bits - 1);
	*en |= mask;
	for(t = en - 1; t >= bn; --t){
		*t = rand();
	}

	return BigInt(ba, num_base, num_base);
}

BigInt BigInt::gen_num_less_than(BigInt & b){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srand(ts.tv_nsec);
	size_t size_b = b.en - b.bn + 1;
	base * ba, * bn, * en, * t, * bt;
	ba = bn = new base[size_b];
	en = bn + size_b - 1;
	if(b == 0){
		return BigInt(0);
	}
	for(t = bn; t <= en; ++t){
		*t = rand();
	}
	if(*en > *(b.en)){
		*en %= *(b.en);
	}
	if(*en != 0 || en == bn){
		if(*en == *(b.en)){
			for(t = en - 1, bt = b.en - 1; t > ba; --t){
				if(*t > *bt){
					*t %= *bt;
					return BigInt(ba, size_b, size_b);
				} else if(*t < *bt){
					return BigInt(ba, size_b, size_b);
				}
			}
			if(en != bn){
				return BigInt(ba, size_b, size_b) - 1;
			} else {
				if(*bn){
					--(*bn);
				} else {
					return BigInt(ba, size_b, size_b) - 1;
				}
			}
			return BigInt(ba, size_b, size_b);
		} else {
			return BigInt(ba, size_b, size_b);
		}
	}

	for(t = en - 1; t > ba && !(*t); --t);
	return BigInt(ba, t - ba + 1, size_b);
}

BigInt_D BigInt::factor(){
	BigInt_D div;
	if(en == bn){
		if(*bn == 0){
			throw "you try to factor 0! are you fucking crazy?";
		} else if(*bn == 1){
			return div;
		}
	}
	if(this -> is_prime()){
		div.push_back(new BP(new BigInt(*this), 1));
		return div;
	}
	BigInt a(*this);
	if(!((*(a.bn) & 1))){
		uint shft = 0;
		for(;*(a.bn) == 0 && a.bn < a.en; shft += BBITS, ++(a.bn));
		base tmp = *(a.bn);
		for(;(tmp & 1) == 0; ++shft, tmp >>= 1);
		a >>= (shft & (BBITS - 1));
		div.push_back(new BP(new BigInt(2), shft));
	}

	if(a.en == a.bn && *(a.bn) == 1){
		return div;
	}


	BigInt  * border;

	border = new BigInt(TRIAL_BORDER);

	if(a.method_trial_div(div, *border)){
		delete border;
		return div;
	}
	
	BP * b = div.back();
	BigInt * num = b -> num;

	if(num -> is_prime()){
		delete border;
		return div;
	}

	div.pop_back();
	b -> num = nullptr;
	delete b;

	std::queue<BP *> q;
	q.push(new BP(num, 1));
	BigInt_D div1;
	for(;!q.empty();){
		b = q.front();
		q.pop();
		div1.clear();
		if(!b -> num -> p_minus_pollard(div1, P_M_BORDER, 10) && !b -> num -> ro_pollard(div1)){
				b -> num -> ferma_with_sift(div1);
		}
		for(auto i = div1.begin(); i != div1.end();){
			if((*i) -> num -> is_prime()){
				div.push_back(*i);
			} else {
				q.push(*i);
			}
			i = div1.erase(i);
			if(i == div1.end()){
				break;
			}
		}
	}
		
	for(auto i = div.begin(); i != div.end(); i++){
		auto j = i;
		j++; 
		for(;j != div.end();){
			if(((*j) -> num) == ((*i) -> num)){
				(*i) -> degree += (*j) -> degree;
				j = div.erase(j);
				if(j == div.end()){
					break;
				}
			} else {
				++j;
			}
		}
	}

	delete border;
	
	return div;
}

bool BigInt::method_trial_div(BigInt_D & div, BigInt & border){
	BigInt * num = this;
	BigInt * d = nullptr, * tmp;
	size_t size_num = (num -> en) - (num -> bn) + 1;
	BigInt * q = new BigInt(size_num,0), * r = new BigInt(size_num, 0), * q2 = new BigInt(size_num, 0);
	auto it = div.begin();
	d = BigInt::next_d(d);
	for(;;){
		BigInt::div_mod(*num, *d, q, r);
		if(r -> en == r -> bn && *(r->bn) == 0){
			if(div.empty()){
				div.push_back(new BP(new BigInt(*d), 1));
			} else {
				it = --div.end();
				if(*((*it) -> num) == *d){
					++((*it) -> degree);
				} else {
					div.push_back(new BP(new BigInt(*d), 1));
				}
			}
			tmp = num;
			num = q;
			q = tmp;
			
			if(num -> bn == num -> en && *(num -> bn) == 1){
				delete r, d;
				if(this == q){
					*q = *num;
					delete num;
				} else delete q;
				return true;
			}

			continue;
		} else {
			if(*q < *d){
				div.push_back(new BP(new BigInt(*num), 1));
				delete r, d;
				if(this == q){
					*q = *num;
					delete num;
				} else delete q;
				return true;
			}
		}
	
		q -> clean();
		r -> clean();
	
		d = BigInt::next_d(d);
		if(*d > border){
			div.push_back(new BP(new BigInt(*num), 1));
			if(this == q){
				*q = *num;
				delete num;
			} else delete q;
			delete r, d;
			return false;
		}
	}
}

BigInt * BigInt::next_d(BigInt * d){
	static bool oe = false;
	if(d){
		if(d -> bn){
			if(d -> en == d -> bn && *(d -> bn) < 7){
				*(d -> bn) += 2;
				return d;
			} else {
				*d += (oe)?(2):(4);
				oe = !oe;
				return d;
			}
		} else {
			throw "bad d";
		}
	} else {
		oe = false;
		return new BigInt(3);
	}
}

void BigInt::clean(){
	base * tmp = ba;
	for(;tmp <= ea;++tmp){
		*tmp = 0;
	}
	bn = en = ba;
}

void BigInt::light_copy(BigInt & b){

	base * tmp, * tmp2;
	for(tmp = ba;tmp <= ea; ++tmp){
		*tmp = 0;
	}
	
	for(tmp = ba, tmp2 = b.bn;tmp <= ea && tmp2 <= b.en; ++tmp, ++tmp2){
		*tmp = *tmp2;
	}
	bn = ba;
	if(tmp > ea && tmp2 <= b.en){
		throw "light copy... you are seriously?";
	}

	en = tmp - 1;
}



int BigInt::jacobi_symb(base a, base p){
	if(p == 0){
		throw "wat";
	}

	if((a = a % p) == 0){
		return 0;
	}

	int res = 1, tmp;
	for(;a != 1;){
		base shft;
		if(!(a & 1)){
			shft = 0;
			for(;(a & 1) == 0 && a;++shft, a >>= 1);
			if(shft & 1){
				if(((((dbase)p) * ((dbase)p) - 1) >> 3) & 1){
					res = -res;
				}
			}
		}
		if(a == 1){
			return res;
		}

		tmp = p % a;
		
		if(!tmp){
			throw "bad p";
		}

		if(((p - 1) >> 1) & ((a - 1) >> 1) & 1){
			res = -res;
		}

		p = a;
		a = tmp;
	}
	return res;
}

int BigInt::jacobi_symb(BigInt & a, base p){
	base a_new = a % p;
	return jacobi_symb(a_new, p);
}

BigInt BigInt::sqrt(){
	size_t num_size = en - bn + 1;
	size_t a_size = (num_size >> 1) + 2;
	BigInt a(a_size,0);
	BigInt * q = new BigInt(a_size, 0);
	q -> bn[a_size - 1] = 1;
	q -> en = q -> bn + a_size - 1;
	
	do{
		a.light_copy(*q);
		BigInt::div_mod(*this, a, q, nullptr);
		*q += a;
		*q >>= 1;
	}while(a > *q);
	
	delete q;

	return a;
}

BigInt BigInt::pow(base n, BigInt & b){
	BigInt a(*this), res(1);
	base tmp, * p1;
	for(;n; n >>= 1){
		if(n & 1){
			res *= a;
			if(res > b){
				return res;
			}
		}
		a = a.sqr();
	}
	return res;
}

BigInt BigInt::pow(base n){
	BigInt a(*this), res(1);
	for(;n; n >>= 1){
		if(n & 1){
			res *= a;
		}
		a = a.sqr();
	}
	return res;
}


void BigInt::root_n(BigInt & res, base n){
	size_t num_size = en - bn + 1;
	size_t res_size = (num_size / n) + 1;
	BigInt * q = new BigInt(res_size, 0);
	q -> bn[res_size - 1] = 1;
	q -> en = q -> bn + res_size - 1;
	
	if(n == 2){
		this -> sqrt(res, q);
		delete q;
		return;
	}
	do{
		res.light_copy(*q);
		BigInt t = res.pow(n - 1, *this);
		BigInt::div_mod(*this, t, q, nullptr);
		*q += res * (n - 1);
		*q = *q / n;
	}while(res > *q);

	delete q;

	return;

}

bool BigInt::is_degree(BigInt & p, base & deg){
	if(en == bn && (*bn == 0 || *bn == 1)){
		throw "not degree";
	}
	base max_n, msk = 0, tmp, count = 1;
	for(msk = 1 << (BBITS - 1), tmp = *en, count = BBITS - 1;msk & tmp; msk >>= 1, --count);
	max_n = count + (en - bn) * BBITS;
	BigInt::resize(p, ((en - bn + 1) >> 1) + 1);
	for(base n = 2;n < max_n; ++n){
		this -> root_n(p, n);
		if((*this) == p.pow(n, *this)){
			deg = n;
			return true;
		}
		p.clean();
	}

	return false;
}


base BigInt::inverse_mod(base a, base p){
	base r, q, b, x1, y1, x2, y2;
	dbase tmp;
	if(a == 0){
		throw "bad a";
	}
	x1 = y2 = 1;
	x2 = y1 = 0;
	b = (base)p;
	q = b / a;
	r = b % a;
	for(;r;){
		tmp = ((dbase)q * (dbase)y2) % p;
		tmp = (y1 + p - tmp) % p;
		y1 = y2;
		y2 = tmp;
		tmp = ((dbase)q * (dbase)x2) % p;
		tmp = (x1 + p - tmp) % p;
		x1 = x2;
		x2 = tmp;		
		b = a;
		a = r;
		q = b / a;
		r = b % a;
	}
	if(a != 1){
		throw "bad prime";
	}
	return (base) y2; 
}


BigInt BigInt::discrete_log(BigInt & g, BigInt & p){
	pol_tup slow(1,0,0), fast(1,0,0);
	BigInt n = p - 1, tmp(0), r(0), rev_tmp(0), nn(0);
	bool fl;
	BigInt m = n.sqrt();
	for(;;){
		F_pollard(slow, *this, g, p, n);
		F_pollard(fast, *this, g, p, n);
		F_pollard(fast, *this, g, p, n);
		if(fast.x == slow.x){
			if(fast.b > slow.b){
				fl = true;
				tmp = fast.b - slow.b;
			}else {
				fl = false;
				tmp = slow.b - fast.b;
			}
			r = tmp.gcd(n);
			std::cout << "n = " << n << std::endl;
			std::cout << "r = " << r << std::endl;
			if(r.en == r.bn && *r.bn == 0){
				slow.y = BigInt::gen_num_less_than(n);
				slow.b = BigInt::gen_num_less_than(n);
				slow.x = g.pow(slow.y, p);
				r = this -> pow(slow.b, p);
				slow.x *= r;
				slow.x = slow.x % p;
				fast.x = slow.x;
				fast.y = slow.y;
				fast.b = slow.b;
				continue;
			}
			if(r > m){
				slow.y = BigInt::gen_num_less_than(n);
				slow.b = BigInt::gen_num_less_than(n);
				slow.x = g.pow(slow.y, p);
				r = this -> pow(slow.b, p);
				slow.x *= r;
				slow.x = slow.x % p;
				fast.x = slow.x;
				fast.y = slow.y;
				fast.b = slow.b;
				continue;
			}
			
			if(r.en == r.bn && *r.bn == 1){
				rev_tmp = tmp.inverse_mod(n);
				if(!fl){
					return ((((n + fast.y) - slow.y)) * rev_tmp) % n; 
				}else {
					return ((((n + slow.y) - fast.y)) * rev_tmp) % n;
				}
			} else {
				tmp = tmp / r;
				nn = n / r;
				rev_tmp = tmp.inverse_mod(nn);
				if(!fl){
					
					tmp = ((((n + fast.y) - slow.y) / r) * rev_tmp) % nn; 
				}else {
					tmp = ((((n + slow.y) - fast.y) / r) * rev_tmp) % nn;
				}
				r = tmp;
				for(;;){
					if(g.pow(tmp, p) == *this){
						return tmp;
					}
					tmp += nn;
					if(tmp > n){
						tmp = tmp % n;
					}
					if(tmp == r){
						throw "woooooooooooooo";
					}

				}
			}
		}
	}
}

void BigInt::F_pollard(pol_tup & tup, BigInt & a, BigInt & g, BigInt & p, BigInt & n){
	base c = tup.x % 3;
	if(c == 1){
		tup.x = (tup.x * a) % p;
		tup.b = (tup.b + 1) % n;
	} else if(c == 2){
		tup.x = tup.x.sqr() % p;
		tup.y = (tup.y << 1) % n;
		tup.b = (tup.b << 1) % n;
	} else {
		tup.x = (tup.x * g) % p;
		tup.y = (tup.y + 1) % n;
	}
}

BigInt BigInt::gcd(BigInt & n){
	base * tmp;
	BigInt a(*this), b(n);
	if(b > a){
		tmp = b.bn;
		b.bn = a.bn;
		a.bn = tmp;
		tmp = b.en;
		b.en = a.en;
		a.en = tmp;
		tmp = b.ba;
		b.ba = a.ba;
		a.ba = tmp;
		tmp = b.ea;
		b.ea = a.ea;
		a.ea = tmp;
	}
	BigInt r = a % b;
	for(;!(r.en == r.bn && *r.bn == 0);){
		a = b;
		b = r;
		r = a % b;
	}
	return b;
}

BigInt BigInt::inverse_mod(BigInt & mod){
	base * tmp;
	BigInt a(mod), y1(0), y2(1), x1(1), x2(0), tmp1(1), q(en - bn + 1, 0), r(en - bn + 1, 0);
	BigInt b = *this % mod;
		
	BigInt::div_mod(a, b, &q, &r);
	for(;!(r.en == r.bn && *r.bn == 0);){
		tmp1 = (q * y2) % mod;
		if(tmp1 > y1){
			tmp1 = (mod + y1) - tmp1;
		} else {
			tmp1 = y1 - tmp1;
		}
		y1 = y2;
		y2 = tmp1;
		tmp1 = (q * x2) % mod;
		if(tmp1 > x1){
			tmp1 = (mod + x1) - tmp1;
		} else {
			tmp1 = x1 - tmp1;
		}
		x1 = x2;
		x2 = tmp1;
		a = b;
		b = r;
		BigInt::div_mod(a, b, &q, &r);
	}
	
	if(!(b.bn == b.en && *b.bn == 1)){
		throw "bad mod";
	}
	return y2;
}

void BigInt::sqrt(BigInt & res, BigInt * q) {
    if(!q) {
        throw "bad q";
    }
    size_t num_size = en - bn + 1;
    size_t res_size = (num_size >> 1) + 2;
    BigInt::resize(res, res_size);
    q -> clean();
    q -> bn[res_size - 1] = 1;
    q -> en = q -> bn + res_size - 1;

    do {
        res.light_copy(*q);
        BigInt::div_mod(*this, res, q, nullptr);
        *q += res;
        *q >>= 1;
    } while(res > *q);

    return;
}

bool BigInt::ferma_with_sift(BigInt_D & div){
	base r = FERMA_BASE;
	std::vector<base> primes = generate_base(r);
	std::vector<base> ki;
	std::vector<uint8_t *> s;
	base tmp, jac;
	uint8_t * t;
	for(auto i = primes.begin(); i != primes.end(); ++i){
		tmp = *this % *i;
		tmp = *i - tmp;
		t = new uint8_t[*i];
		for(dbase j = 0; j < *i; ++j){
			jac = (j * j + tmp) % *i;
			if(-1 != jacobi_symb(jac, *i)){
				t[j] = 1;
			} else {
				t[j] = 0;
			}
		}
		s.push_back(t);
	}

	BigInt x = this -> sqrt(), z(0), y(0);

	if(x.sqr() == *this){
		div.push_back(new BP(new BigInt(x), 2));
		return false;
	}
	x += 1;
	BigInt tall = (*this + 1) >> 1;
	for(auto i = primes.begin(); i != primes.end(); ++i){
		ki.push_back(x % *i);
	}
	for(;;){
		base l = 0;
		bool fl = true;
		for(auto i = ki.begin();i != ki.end(); ++i, ++l){
			if(s[l][*i] == 0){
//				std::cout << "GO away" << std::endl;
				fl = false;
				break;
			}
		}
		if(fl){
//			std::cout << "OK" << std::endl;
			z = x.sqr() - *this;
			y = z.sqrt();
			if(y.sqr() == z){
				z = x + y;
				div.push_back(new BP(new BigInt(z), 1));
				z = x - y;
				div.push_back(new BP(new BigInt(z), 1));
				for(auto si = s.begin();si != s.end();++si){
					delete *si;
				}
				return false;
			}
		}
		x += 1;
	//	std::cout << x << std::endl;
		if(!(x < tall)){
			for(auto si = s.begin();si != s.end();++si){
				delete *si;
			}
			return true;
		}
		for(auto i = ki.begin(), it = primes.begin();i != ki.end() && it != primes.end(); ++i, ++it){
			*i = (*i + 1) % *it;
		}
	}
}

std::vector<base> BigInt::generate_base( base k){
		bool fl = false;
		std::vector<base> primes;
		primes.push_back(2);
		base count;
		for(base c = 3, count = 1; count < k;c += 2){
				fl = true;
				for(auto i = primes.begin();i != primes.end(); ++i){
					if((c % *i) == 0){
						fl = false;
						break;
					}
				}
				if(fl){
					primes.push_back(c);
					count++;
					fl = false;
				}
		}
		return primes;
}

bool BigInt::ro_pollard(BigInt_D & div){
	BigInt a(2), b(2), c(1), d(1), buf1(en - bn + 1, 0), buf2(en - bn + 1, 0);
	for(;;){
		a = a.sqr();
		a += c;
		a = a % *this;
		b = b.sqr();
		b += c;
		b = b % *this;
		b = b.sqr();
		b += c;
		b = b % *this;
		if(b == a){
			return false;
		}
		if(a < b){
			d = (b - a).gcd(*this);
		} else {
			d = (a - b).gcd(*this);
		}
		if(!(d.en == d.bn && *d.bn == 1)){
			div.push_back(new BP(new BigInt(d), 1));
			d = *this / d;
			div.push_back(new BP(new BigInt(d), 1));
			return true;
		}
	}
}

bool BigInt::p_minus_pollard(BigInt_D & div, base B, base lim){
	std::vector<base> primes;
	BigInt a(en - bn + 1, 0);
	BigInt n_m(*this);
	n_m -= 1;
	BigInt d;
	BigInt q(en - bn + 1, 0);
	base l;
	size_t size_n = en - bn + 1, size_e;
	base log_2_n;
	base shft = 31, mask = ((base)1) << (BBITS - 1);
	
	generate_base_less_border(primes, B);
	
	for(;!(mask & *en);--shft, mask >>= 1);
	log_2_n = size_n * BBITS + shft;
	
	for(;;){
		a = BigInt::gen_num_less_than(n_m);
		for(auto i = primes.begin(); i != primes.end(); ++i){
			shft = 31, mask = ((base)1) << (BBITS - 1);
			for(;!(mask & *i);--shft, mask >>= 1);
			l = log_2_n / shft + 1;
			q.clean();
			*q.bn = *i;
			q = q.pow(l);
			a = a.pow(q, *this);
		}

		if(a.en == a.bn && *a.bn == 1){
			continue;
		}

		d = (a - 1).gcd(*this);
		
		if((d.bn != d.en || *d.bn > 1) && d < *this){
			BigInt q = *this / d;
			div.push_back(new BP(new BigInt(q), 1));
			div.push_back(new BP(new BigInt(d), 1));
			break;
		} else {
			if(lim == 0){
				return false;
			} else {
				lim--;
			}
			l = B + B_STEP;
			if(l > B){
				B = l;
			} else {
				return false;
			}
			generate_base_less_border(primes, B);
		}
	}
	return true;
}

void BigInt::generate_base_less_border(std::vector<base> & primes, base B){
		bool fl = false;
		base l;
		if(primes.empty()){
			primes.push_back(2);
			l = 3;
		} else {
			l = primes.back();
			if(!(l & 1)){
				l = 3;
			}
		}

		for(base c = l; c < B;c += 2){
				fl = true;
				for(auto i = primes.begin();i != primes.end(); ++i){
					if((c % *i) == 0){
						fl = false;
						break;
					}
				}
				if(fl){
					primes.push_back(c);
					fl = false;
				}
		}

}


BigInt BigInt::primitive_root_modulo(BigInt & p){
	BigInt n = p - 1;
	BigInt a = BigInt::gen_num_less_than(n);
	BigInt_D div = n.factor();
	BigInt e(n.en - n.bn + 1, 0);
	BigInt b;

	if(a.bn == a.en && *a.bn == 1){
		a += 1;
	}

	bool fl = true;

	for(;;){
		for(auto i = div.begin(); i != div.end(); ++i){
			div_mod(n, *((*i) -> num), &e, nullptr);
			b = a.pow(e, p);
			if(b.en == b.bn && *b.bn == 1){
				fl = false;
				break;
			}
		}
		if(fl){
			return a;
		} else {
			a += 1;//BigInt::gen_num_less_than(n);
			if(!(a < n)){
				a = BigInt::gen_num_less_than(n);
			}
			fl = true;
		}
	}
}

bool cmpBP(BP * x, BP * y){
    return *(x -> num) < *(y -> num);
}

