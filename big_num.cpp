#include <iostream>
#include <big_num.h>

BigNumber::BigNumber() {
    bn = en = ea = ba = NULL;
}

BigNumber::~BigNumber() {
    if (ba) {
        delete [] ba;
    }
    bn = en = ea = ba = NULL;
}

BigNumber::BigNumber (base *b, size_t size, size_t r_size) {
    ba = bn = b;
    en = b + size - 1;
    ea = b + r_size - 1;
    for (; en > bn && !(*en); --en);
}

BigNumber * BigNumber::get_degree_two (size_t num_base, size_t num_bit) {
    if (num_bit < 0 || num_bit > BBITS) throw "bad num_bits";
    base *bb = new base[num_base];
    base *now;
    now = bb;

    for (int i = 0; i < num_base - 1; ++i, ++now) {
        *now = (base)0;
    }
    *now = 1 << num_bit;
    return new BigNumber (bb, num_base, num_base);
}

BigNumber::BigNumber(size_t len, base fill) {
    ba = new base[len];
    if (!ba) {
        throw ALLOC_ERR;
    }
    bn = ba;
    ea = ba + len - 1;

    for (int i = 0; i < len; ++i) {
        bn[i] = fill;
    }
    if (fill != 0) {
        en = ea;
    }
    else {
        en = bn;
    }
    /*for (base *tmp = ba; tmp <= ea; ++tmp) {
        *tmp = fill;
    }*/
}

BigNumber::BigNumber(base b) {
    ba = new base[1];
    if (!ba) {
        throw ALLOC_ERR;
    }
    bn = en = ea = ba;
    bn[0] = b;
}

BigNumber::BigNumber(int b) {
    ba = new base[1];
    if (!ba) {
        throw ALLOC_ERR;
    }
    bn = en = ea = ba;
    bn[0] = (base)b;
}

BigNumber::BigNumber(const BigNumber& b) {
    /*if (b.ba == NULL) {
        bn = en = ea = ba = NULL;
        return;
    }
    size_t len = b.en - b.bn + 1;
    ba = new base[len];
    if (!ba) {
        throw ALLOC_ERR;
    }
    bn = ba;
    ea = en = ba + len - 1;
    for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 <= en; ++tmp1, ++tmp2) {
        *tmp1 = *tmp2;
    }*/
    int size = b.en - b.bn + 1;
    ba = new base[size];
    if (!ba) throw ALLOC_ERR;
    ea = ba + size - 1;
    en = ea;
    bn = ba;
    for (int i = 0; i < size; ++i) {
        bn[i] = b.bn[i];
    }
}

BigNumber & BigNumber::operator=(const BigNumber& b) {
    if (this == &b) return *this;
    if (b.ba == NULL) {
        bn = en = ea = ba = NULL;
        return *this;
    }
    size_t len = b.en - b.bn + 1;
    ba = new base[len];
    if (!ba) {
        throw ALLOC_ERR;
    }
    bn = ba;
    ea = en = ba + len - 1;
    for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 <= en; ++tmp1, ++tmp2) {
        *tmp1 = *tmp2;
    }

}

int BigNumber::Compare(const BigNumber& b) {
    if ((en - bn) == (b.en - b.bn)) {
        for (base *tmp1 = en, *tmp2 = b.en; tmp1 >= bn; --tmp1, --tmp2) {
            if (*tmp1 > *tmp2) return 1;
            if (*tmp1 < *tmp2) return -1;
    }
        return 0;
}
    else {
        if ((en - bn) > (b.en - b.bn)) return 1;
        else return -1;
    }
}

bool BigNumber::operator < (const BigNumber& b) {
    return (Compare(b) == -1);
}

bool BigNumber::operator > (const BigNumber& b) {
    return (Compare(b) == 1);
}

bool BigNumber::operator >= (const BigNumber& b) {
    return (Compare(b) == 1 || Compare(b) == 0);
}

bool BigNumber::operator <= (const BigNumber& b) {
    return (Compare(b) == -1 || Compare(b) == 0);
}

bool BigNumber::operator == (const BigNumber& b) {
    return (Compare(b) == 0);
}

bool BigNumber::operator != (const BigNumber& b) {
    return (Compare(b) != 0);
}

BigNumber BigNumber::operator+(const BigNumber& b) const {
    size_t size_max;
    size_t size_min;
    int i;
    base *min, *max;
    if (en - bn >= b.en - b.bn) {
        size_max = en - bn + 1;
        size_min = b.en - b.bn + 1;
        max = bn;
        min = b.bn;
    }
    else {
        size_max = b.en - b.bn + 1;
        size_min = en - bn + 1;
        max = b.bn;
        min = bn;
    }
    BigNumber res(size_max + 1, 0);
    dbase buffer = 0;
    for (i = 0; i < size_min; ++i) {
        buffer += (dbase)min[i] + (dbase)max[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }
    for (; i < size_max; ++i) {
        buffer += (dbase)max[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }
    if (buffer) {
        res.bn[size_max] = (base)buffer;
        res.en = res.bn + size_max;
    } else res.en = res.bn + size_max - 1;
    
    return res;
}

BigNumber BigNumber::operator-(const BigNumber& b) const {
    //if (*this < b) throw "< 0";
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if (size_b > size_a) throw "<0";
    int i;
    dbase buffer = 0;
    BigNumber res(size_a, 0);
    for (i = 0; i < size_b; ++i) {
        buffer = (dbase)bn[i] - (dbase)b.bn[i] - buffer;
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
        buffer = (buffer)?1:0;
    }
    for (; i < size_a && buffer; ++i) {
        buffer = (dbase)bn[i] - buffer;
        res.bn[i] = (base)buffer;
        buffer >>=BBITS;
        buffer = (buffer)?1:0;
    }
    for (; i < size_a; ++i) res.bn[i] = bn[i];
    if (i == size_a && !res.bn[size_a - 1] && size_a != 1) {
        res.en = res.bn + size_a - 2;
        for (; *res.en == 0 && res.en > res.bn; --res.en);
    } else res.en = res.bn + size_a - 1;

    return res;
}

void BigNumber::plus(const BigNumber& b, BigNumber& res) const {
    size_t size_max;
    size_t size_min;
    base *max, *min;
    int i;
    if (en - bn >= b.en - b.bn) {
        size_max = en - bn + 1;
        size_min = b.en - b.bn + 1;
        max = bn;
        min = b.bn;
    }
    else {
        size_max = b.en - b.bn + 1;
        size_min = en - bn + 1;
        max = b.bn;
        min = bn;
    }

    dbase buffer = 0;
    for (i = 0; i < size_min; ++i) {
        buffer += (dbase)min[i] + (dbase)max[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }
    for (; i < size_max; ++i) {
        buffer += (dbase)max[i];
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }
    if (buffer) {
        res.bn[size_max] = (base)buffer;
        res.en = res.bn + size_max;
    } else res.en = res.bn + size_max - 1;

    return;
}

BigNumber & BigNumber::operator += (const BigNumber& b) {
    ReSize(*this, en - bn + 2);
    this -> plus(b, *this);
    return *this;
}

void BigNumber::minus(const BigNumber& b, BigNumber& res) {
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if (size_b > size_a) throw "< 0";
    int i;
    dbase buffer = 0;
    //BigNumber res(*this);

    for (i = 0; i < size_b; ++i) {
        buffer = (dbase)bn[i] - (dbase)b.bn[i] - buffer;
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
        buffer = (buffer)?1:0;
    }
    for (; i < size_a && buffer; ++i) {
        buffer = (dbase)bn[i] - buffer;
        res.bn[i] = (base)buffer;
        buffer >>=BBITS;
        buffer = (buffer)?1:0;
    }
    for (; i < size_a; ++i) res.bn[i] = bn[i];

    if (buffer > 0) throw "< 0";

    if (i == size_a && !res.bn[size_a - 1] && size_a != 1) {
        res.en = res.bn + size_a - 2;
        for (; *res.en == 0 && res.en > res.bn; --res.en);
    } else res.en = res.bn + size_a - 1;

    return;
}

BigNumber & BigNumber::operator -= (const BigNumber& b) {
    this -> minus(b, *this);
    return *this;
}

void BigNumber::ReSize(BigNumber& b, size_t new_size) {
    int i;
    size_t size_b = b.en - b.bn + 1;
    if (new_size <= 0) {
        throw "bad size";
    }
    if (new_size > size_b) {
        if (new_size > b.ea - b.ba + 1) {
            base * a = new base[new_size];
            if(!a) throw ALLOC_ERR;
            for (i = 0;i < size_b; ++i) {
                a[i] = b.bn[i];
            }
            for (;i < new_size; ++i){
                a[i] = 0;
            }
            delete[] b.ba;
            b.ba = b.bn = a;
            b.en = a + size_b - 1;
            b.ea = a + new_size - 1;
        }
        else {
            for (i = 0; i < size_b; ++i) {
                b.ba[i] = b.bn[i];
            }
            for (base * tmp = b.ba + i; tmp <= b.ea; *tmp = 0, ++tmp){
                b.ba[i] = 0;
                b.en = b.ba + size_b - 1;
            }
        }
    }
}

BigNumber BigNumber::operator * (BigNumber& b) {
    return this -> Usual_mul(b);
}

BigNumber BigNumber::Usual_mul (BigNumber &b) {
    dbase buffer = 0;
    int i, j;
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if (size_a == 1 && *bn == 0 || size_b == 1 && *b.bn == 0) return BigNumber(0);
    
    BigNumber res(size_a + size_b, 0);
    
    for (i = 0; i < size_b; ++i){
        for (j = 0; j < size_a; ++j) {
            buffer += (dbase)(b.bn[i]) * (dbase)(bn[j]) + (dbase)(res.bn[i + j]);
            res.bn[i + j] = (base)buffer;
            buffer >>= BBITS;
        }
        for (; buffer && j < size_a + size_b - i; ++j) {
            buffer += res.bn[i + j];
            res.bn[i + j] = (base)buffer;
            buffer >>= BBITS;
        }
    }
    if(res.bn[size_a + size_b - 1]) {
        res.en = res.bn + size_a + size_b - 1;
    }
    else {
        res.en = res.bn + size_a + size_b - 2;
    }
    
    return res;
}

BigNumber BigNumber::operator >> (int num) const{
    
    if (num < 0 || num > BBITS - 1) throw "so big";

    base mask, prev_buffer = 0, buffer = 0;
    BigNumber res(*this);

    mask = BASE_MASK(num);
    for (int i = en - bn; i >= 0; --i) {
        buffer = res.bn[i] & mask;
        res.bn[i] = (res.bn[i] >> num) | (prev_buffer << (BBITS - num));
        prev_buffer = buffer;
    }

    if (!(*res.en) && res.en != res.bn) --res.en;

    return res;
}

BigNumber BigNumber::operator << (int num) const {
    
    if (num < 0 || num > BBITS - 1) throw "so big";

    base mask, prev_buffer = 0, buffer = 0;
    BigNumber res(*this);

    mask = BASE_MASK(num);
    for (int i = 0; i >= en - bn + 1; ++i) {
        buffer = (res.bn[i] >> (BBITS - num)) & mask;
        res.bn[i] = (res.bn[i] << num) | prev_buffer;
        prev_buffer = buffer;
    }

    if (!(*res.en) && res.en != res.bn) {
        BigNumber::ReSize(res, en - bn + 2);
        ++res.en;
        *res.en = prev_buffer;
    }

    return res;
}

BigNumber & BigNumber::operator >>= (int num) {
    if (num < 0 || num > BBITS - 1) throw "so big";
    
    base buffer = 0, prev_buf = 0, mask = (1 << num) - 1;

    for (int i = en - bn; i >= 0; --i) {
        buffer = bn[i] & mask;
        bn[i] = (bn[i] >> num) | (prev_buf << (BBITS - num));
        prev_buf = buffer;
    }

    if (!*en && en != bn) --en;
    
    return *this;
}

BigNumber & BigNumber::operator <<= (int num) {
    if (num < 0 || num > BBITS - 1) throw "so big";

    base buffer = 0, prev_buf = 0, mask = (1 << num) - 1;
    for (int i = 0; i < en - bn + 1; ++i) {
        buffer = (bn[i] >> (BBITS - num)) & mask;
        bn[i] = (bn[i] << num) | prev_buf;
        prev_buf = buffer;
    }
    if (prev_buf) {
        BigNumber::ReSize(*this, en - bn + 2);
        ++en;
        *en = prev_buf;
    }
    
    return *this;
}


void BigNumber::division_base(const BigNumber& a, base b, BigNumber *q, base *r) {
    if (b == 0) throw "division by zero";

    dbase buffer = 0, tmp;
    size_t size_a = a.en - a.bn + 1;

    if (size_a == 1) {
        base old  = (*a.bn);
        if (old) {
            if (q) {
                q -> en = q -> bn;
                *(q -> bn) = old / b;
            }
            if (r) {
                *r = old % b;
            }
            return;
        }
        else {
            if (q) {
                q -> en = q -> bn;
                *(q -> bn) = 0;
            }
            if (r) {
                *r = 0;
            }
            return;
        }
    }

    if (q) BigNumber::ReSize(*q, size_a);

    for (int j = size_a - 1; j >= 0; --j) {
        buffer = (buffer << BBITS) | a.bn[j];
        tmp = buffer / (dbase)b;
        if (q) q -> bn[j] = (dbase)tmp;
        buffer = buffer - (dbase)b * tmp;
    }

    if (r) {
        *r = (dbase)buffer;
    }

    if (q) {
        if (q -> bn[size_a - 1] != 0) {
            q -> en = q -> bn + size_a - 1;
        }
        else q -> en = q -> bn + size_a - 2;
    }
}

BigNumber BigNumber::operator / (base b) const {
    BigNumber res(en - bn + 1, 0);
    division_base(*this, b, &res, NULL);
    return res;
}

BigNumber BigNumber::operator * (base b) {
    dbase buffer = 0;
    int i;
    size_t  size_a = en - bn + 1;
    if (b == 0 || size_a == 1 && *bn == 0) {
        return BigNumber(0);
    }

    BigNumber res(size_a + 1, 0);

    for (i = 0; i < size_a; ++i) {
        buffer += (dbase)bn[i] * b;
        res.bn[i] = (base)buffer;
        buffer >>= BBITS;
    }
    if (buffer) {
        res.bn[size_a] = (base)buffer;
        res.en = res.bn + size_a;
    }
    else res.en = res.bn + size_a - 1;

    return res;
}

BigNumber & BigNumber::operator *= (base b) {
    dbase buffer = 0;
    int i;
    size_t  size_a = en - bn + 1;
    
    if (b == 0 || size_a == 1 && *bn == 0) {
        return *this;
    }

    BigNumber::ReSize(*this, size_a + 1);

    for (i = 0; i < size_a; ++i) {
        buffer += (dbase)bn[i] * b;
        bn[i] = buffer;
        buffer >>= BBITS;
    }
    if (buffer) {
        bn[size_a] = buffer;
        en = bn + size_a;
    }
    else en = bn + size_a - 1;

    return *this;
}

std::istream & operator >> (std::istream& in, BigNumber& b) {
    std::string str;
    int i;
    in >> str;
    BigNumber::ReSize(b, str.size() / 4 + 1);

    for (i = 0; i < str.size()/4 + 1; ++i) {
        b.bn[i] = 0;
    }
    b.en = b.bn;
    for (i = 0; i < str.size(); ++i) {
        if (str[i] < '0' || str[i] > '9') {
            throw "bad string";
        }
        b *= (base)10;
        b += (base)(str[i] - '0');
    }

    return in;
}

BigNumber::BigNumber(const std::string & str) {
    int i;
    en = bn = ba = new base[str.size() / 4 + 1];
    if (!ba) {
        throw ALLOC_ERR;
    }
    ea = ba + str.size() / 4;
    *bn = 0;

    for (i = 0; i < str.size(); ++i) {
        if (str[i] < '0' || str[i] > '9') {
            throw "bad string";
        }
        *this *= (base)10;
        *this += (base)(str[i] - '0');
    }
}

std::ostream & operator << (std::ostream& out, const BigNumber& b) {
    std::stack<char> st;
    BigNumber copy(b);
    base num;
    
    if (copy.en == copy.bn && !*copy.bn) {
        out << "0";
        return out;
    }

    for (;copy.en - copy.bn > 0 || *copy.bn;) {
        BigNumber::division_base(copy, (base)10, &copy, &num);
        st.push((char)(num + '0'));
    }

    for (;!st.empty(); st.pop()) {
        out << st.top();
    }

    return out;
}

void BigNumber::div_mod (BigNumber& a, BigNumber& b, BigNumber *q, BigNumber *r) {
    if (b == 0) throw "division on zero";
    
    int i, count = 0, c1 = BBITS - 1;
    dbase c2 = (dbase)1 << BBITS;
    size_t size_a = a.en - a.bn + 1, size_b = b.en - b.bn + 1;
    
    if (size_b == 1) {
        division_base(a, *(b.bn), q, (r)?r -> bn:nullptr);
        if (r) r -> en = r -> bn;
        return;
    }
    else {
        if (a < b) {
            if (r) {
                for (i = 0; i < size_a; ++i) {
                    r -> bn[i] = a.bn[i];
                }
                r -> en = r -> bn + size_a - 1;
            }
            if (q) {
                *(q -> bn) = 0;
                q -> en = q -> bn;
            }
            return;
        }
    base *q1;
    base *ub = new base[size_a + 5];
    if (!ub) throw ALLOC_ERR;
    ub[size_a + 1] = ub[size_a] = 0;
    for (i = 0; i < size_a; ++i) {
        ub[i] = a.bn[i];
    }
    BigNumber u(ub, size_a, size_a + 2);
    base mask = (base)(1 << c1);
    for (i = c1; i >= 0; mask >>= 1, --i) {
        if (*(b.en) & mask) {
            count = c1 - i;
            i = -1;
        }
    }

    b <<= count;
    u <<= count;

    dbase qhat, rhat;
    size_a = u.en - u.bn + 1;
    size_b = b.en - b.bn + 1;

    if (q) q1 = q -> en = q -> bn + size_a - size_b;
    if (size_a > size_b) u.bn = u.en - size_b + 1;

    for (int j = size_a; j >= size_b; --j) {
        qhat = ((((dbase)u.bn[size_b]) << BBITS) + u.bn[size_b - 1]);
        rhat = qhat % (*b.en);
        qhat /= (*b.en);
        for (; rhat < c2 && qhat *(*(b.en - 1)) > (rhat << BBITS) + u.bn[size_b + 2];) {
            --qhat;
            rhat += *(b.en);
        }
        if (b * (base)qhat > u) {
            --qhat;
            u += b;
        }
        u.en = u.bn + size_b;
        u -= b * qhat;
        --u.bn;
        if (q) {
            *q1 = qhat;
            --q1;
        }
    }

    b >>= count;

    if (r) {
        ++u.bn;
        u >>= count;
        for (i = 0; i < u.en - u.bn + 1; ++i) {
            r -> bn[i] = u.bn[i];
        }
        r -> en = r -> bn + (u.en - u.bn);
        if (!(b > *r)) *r -= b;
    }
    if (q) {
        ++q1;
        q -> bn = q1;
        for (; !*(q -> en); --q -> en);
    }
  }
}

BigNumber BigNumber::operator / (BigNumber& b) {
    if (en - bn >= b.en - b.bn) {
        BigNumber res((en - bn) - (b.en - b.bn) + 2, 0);
        div_mod(*this, b, &res, nullptr);
        return res;
    }
    else {
        return BigNumber(0);
    }
}

BigNumber BigNumber::operator % (BigNumber& b) {
    if (en - bn >= b.en - b.bn) {
        BigNumber res(b.en - b.bn + 1, 0);
        div_mod(*this, b, nullptr, &res);
        return res;
    }
    else {
        return BigNumber(*this);
    }
}

BigNumber BigNumber::sqr() {
    return *this * *this;
}

BigNumber BigNumber::pow(BigNumber & degree, BigNumber & mod) {
    BigNumber res(1), z(*this), num;
    base *end, *now, tmp;
    size_t size_mod = mod.en - mod.bn + 1;
    int i;
    BigNumber *d2 = BigNumber::get_degree_two((size_mod << 1) + 1, 0);
    num = (*d2) / mod;
    delete d2;
    z = z % mod;

    for (now = degree.bn; now <= degree.en; ++now) {
        tmp = *now;
        if (tmp) {
            for (i = 0; i < BBITS; ++i, tmp >>= 1) {
                if (tmp & 1) {
                    res = res * z;
                    res.barret(mod, num);
                    //res = res % mod;
                }
                z = z.sqr();
                res.barret(mod, num);
                //z = z % mod;
            }
        }
        else {
            for (i = 0; i < BBITS; ++i) {
                z = z.sqr();
                res.barret(mod, num);
                //z = z % mod;
            }
        }
    }
    return res;
}

BigNumber & BigNumber::barret (BigNumber & mod, BigNumber & num) {
    if (mod == 0) throw "bad mod";

    size_t size_mod = mod.en - mod.bn + 1;
    base *end = en, *now, *now_r;
    base *tmp = new base[size_mod + 2];

    if (*this < mod) return *this;
    BigNumber rem (*this);
    BigNumber r1 (tmp, size_mod + 1, size_mod + 2);
    en = end;
    bn += size_mod + 1;
    *this = *this * num;
    BigNumber nn(rem);
    rem = rem / mod;
    *this = *this * mod;
    if (en - bn > size_mod) en = bn + size_mod;

    for (now = bn + size_mod + 1; now <= ea; *now = 0, ++now);
    for (; *en == 0 && bn < en; --en);
    
    if (!(*this > r1)) {
        r1 -= *this;
    }
    else {
        size_t size_this = en - bn + 1;
        base mask = ((base)1) << (BBITS - 1);
        base shift = BBITS - 1;
        
        for (; !(mask & *en); mask >>= 1, --shift);
        BigNumber::ReSize (r1, size_mod + 2);
        size_this = (size_this > size_mod)?(size_mod):(size_this);
        r1.en = r1.bn + size_this;
        *(r1.en) += 1 << shift;
        r1 -= *this;
    }

    for (; !(r1 < mod); ){
        r1 -= mod;        
    }
    delete[] ba;
    ba = r1.ba;
    bn = r1.bn;
    ea = r1.ea;
    en = r1.en;
    r1.ba = nullptr;
    return *this;
}
