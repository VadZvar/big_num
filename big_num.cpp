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

int BigNumber::Compare(const BigNumber& b) const{
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

bool BigNumber::operator < (const BigNumber& b) const {
    return (Compare(b) == -1);
}

bool BigNumber::operator > (const BigNumber& b) const {
    return (Compare(b) == 1);
}

bool BigNumber::operator >= (const BigNumber& b) const {
    return (Compare(b) == 1 || Compare(b) == 0);
}

bool BigNumber::operator <= (const BigNumber& b) const {
    return (Compare(b) == -1 || Compare(b) == 0);
}

bool BigNumber::operator == (const BigNumber& b) const {
    return (Compare(b) == 0);
}

bool BigNumber::operator != (const BigNumber& b) const {
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

BigNumber BigNumber::operator - (const BigNumber& b) const {
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
        buffer >>= BBITS;
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

/*void BigNumber::ReSize(BigNumber& b, size_t new_size) {
    int i;
    size_t size_b = b.en - b.bn + 1;
    if (new_size <= 0) {
        throw "bad size";
    }
    if (new_size > size_b) {
        if (new_size > b.ea - b.ba + 1) {
            base * a = new base[new_size];
            if(!a) throw ALLOC_ERR;
            for (i = 0; i < size_b; ++i) {
                a[i] = b.bn[i];
            }
            for (; i < new_size; ++i){
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
           //for (base * tmp = b.ba + i; tmp <= b.ea; *tmp = 0, ++tmp){
                b.ba[i] = 0;
                b.en = b.ba + size_b - 1;
            }
            for (; i < new_size; ++i) {
                b.ba[i] = 0;
            }
            b.bn = b.ba;
            b.en = b.bn + size_b - 1;
        }
    } else {
        b.en = b.bn + new_size - 1;
    }
}*/

void BigNumber::ReSize(BigNumber & b, size_t new_size) {
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
            for(i = 0; i < size_b; i++) {
                b.ba[i]	 = b.bn[i];
            }
            for(; i < new_size; i++) {
                b.ba[i] = 0;
            }
            b.bn = b.ba;
            b.en = b.bn + size_b - 1;
        }
    } else {
        b.en = b.bn + new_size - 1;
    }
}

BigNumber BigNumber::operator * (const BigNumber& b) {
    //return this -> Usual_mul(b);
    return this -> fmul(b);
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

void BigNumber::usual_mul_mem (const BigNumber &b, BigNumber &res) {
    dbase buffer = 0;
    int i, j;
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
    if (size_a == 1 && *bn == 0 || size_b == 1 && *b.bn == 0) {
        res.en = res.bn;
        *(res.bn) = 0;
        return;
    }
    
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
    
    return;
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


void BigNumber::div_mod(const BigNumber& a, base b, BigNumber *q, base *r) {
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
    div_mod (*this, b, &res, nullptr);
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

BigNumber & BigNumber::operator *= (BigNumber& b) {
    return (*this = this -> fmul(b));
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
        BigNumber::div_mod(copy, (base)10, &copy, &num);
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
        div_mod (a, *(b.bn), q, (r)?r -> bn:nullptr);
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
                    res = res.barret(mod, num);
                    //res = res % mod;
                }
                z = z.sqr();
                z.barret(mod, num);
                //z = z % mod;
            }
        }
        else {
            for (i = 0; i < BBITS; ++i) {
                z = z.sqr();
                z.barret(mod, num);
                //z = z % mod;
            }
        }
    }
    return res;
}

BigNumber & BigNumber::barret (const BigNumber & mod, BigNumber & num){
	size_t size_mod = mod.en - mod.bn + 1;
	if (mod == 0) throw "bad mod";
	if (*this < mod) {
		return *this;
	}
    base *end = en;
    en = bn + size_mod - 1;
    for (;*en == 0 && bn < en; --en); //идём до первой ненулевой базы
	BigNumber r1(*this);
    en = end;
	bn += size_mod - 1;
	*this = *this * num;
	bn += size_mod + 1;
	*this = *this * mod;
	en = bn + size_mod - 1;
    for (; *en == 0 && bn < en; --en);

	if (!(*this > r1)) {
		*this = r1 - *this;
	} 
    else {
		BigNumber::ReSize(r1, size_mod + 1);
		r1.en = r1.bn + size_mod;
		*(r1.en) = 1; //b^k+1 + r1
		*this = r1 - *this; //-r2
	}
	for (;!(*this < mod);) {
		*this = *this - mod;
	}
	return *this;
}

BigNumber BigNumber::fmul (const BigNumber& b) {
    size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;
    BigNumber res(size_a + size_b + 1, 0);

    this -> fmul_car(b, res);
    return res; 
}

void BigNumber::fmul_car (const BigNumber& b, BigNumber& res) {
	size_t size_a = en - bn + 1, size_b = b.en - b.bn + 1;
	if(CAR_BORDER < 3) {
		throw "bag CAR_BORDER";
	}
	
	if(size_a > CAR_BORDER && size_b > CAR_BORDER) {
		size_t mask = (size_a > size_b) ? (size_a >> 1) : (size_b >> 1);//половина макс.размера числа
		if (mask < size_a && mask < size_b) {
			BigNumber a1(bn + mask, size_a - mask, size_a - mask),\
					  a2(bn, mask, mask),\
					  b1(b.bn + mask, size_b - mask, size_b - mask),\
					  b2(b.bn, mask, mask);
			
			size_t  size_a1 = a1.en - a1.bn + 1, \
					size_a2 = a2.en - a2.bn + 1, \
					size_b1 = b1.en - b1.bn + 1, \
					size_b2 = b2.en - b2.bn + 1;
			size_t mm1 = (size_a1 > size_b1) ? (size_a1 + 1) : (size_b1 + 1),\
				   mm2 = (size_a2 > size_b2) ? (size_a2 + 1) : (size_b2 + 1);
			size_t num = (mask << 1) + 4 + size_a1 + size_a2 + size_b1 + size_b2 + mm1 + mm2;
							
			base * tmp = new base[num];
            if (!(*tmp)) throw ALLOC_ERR;
			base * now;	
			for (now = tmp; now < tmp + num; ++now) {
				*now = 0;
			}
			now = tmp;
			BigNumber r1(now, 1, mask + 1);
			now += mask + 1;
			BigNumber r2(now, 1, mask + 1);
			now += mask + 1;
			BigNumber r3(now, 1, size_a1 + size_b1 + 1);
			now += size_a1 + size_b1;
			BigNumber r4(now, 1, size_a2 + size_b2 + 1);
			now += size_a2 + size_b2;
			BigNumber r5(now, 1, mm1 + mm2 + 1);


			a1.plus(a2, r1);
			b1.plus(b2, r2);
			
			a2.fmul_car(b2, r4);//A
			a1.fmul_car(b1, r3);//B
			r1.fmul_car(r2, r5);//C
			
			r5 -= r3;
			r5 -= r4;
			
			base * now_res, * now_r;

			for (now_res = res.bn, now_r = r4.bn; now_r <= r4.en; ++now_r, ++now_res) {
				*now_res = *now_r;
			}

			now_res = res.bn + mask;
			
			dbase buffer = 0;
			
			for (now_r = r5.bn; now_r <= r5.en; ++now_r, ++now_res) {
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for (; buffer; ++now_res) {
				buffer += (dbase) *now_res;
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			now_res = res.bn + (mask << 1);

			for (now_r = r3.bn; now_r <= r3.en; ++now_r, ++now_res) {
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for (; buffer; ++now_res) {
				buffer += (dbase)*now_res;
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for (now_res = res.bn + size_a + size_b - 1; !(*now_res); --now_res);
			res.en = now_res;

			a1.ba = a2.ba = b1.ba = b2.ba = r1.ba = r2.ba = r3.ba = r4.ba = r5.ba = nullptr;
			delete [] tmp;
		} else {
			BigNumber max1, max2, min;
			size_t size_min, size_max1, size_max2, num; 
			if (size_a > size_b) {
				max1.ba = max1.bn = bn + mask;
				max1.ea = max1.en = en;
				max2.ba = max2.bn = bn;
				max2.ea = max2.en = bn + mask - 1;
				for (; max2.bn < max2.en && !(*max2.en); --(max2.en));
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
            if (!(*tmp)) throw ALLOC_ERR;

			for (now = tmp; now < tmp + num; ++now) {
				*now = 0;
			}
			
			now = tmp;
			BigNumber r1(now, 1, size_max1 + size_min);
			now += size_min + size_max1;
			BigNumber r2(now, 1, size_max2 + size_min);

			max1.fmul_car(min, r1);
			max2.fmul_car(min, r2);
			
			base * now_res = res.bn, * now_r;
			
			dbase buffer = 0;

			for (now_r = r2.bn; now_r <= r2.en; ++now_r, ++now_res) {
				*now_res = *now_r;
			}

			now_res = res.bn + mask;

			for (now_r = r1.bn; now_r <= r1.en; ++now_r, ++now_res) { 
				buffer += (dbase)(*now_res) + (dbase)(*now_r);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for (; buffer; ++now_res) {
				buffer += (dbase)(*now_res);
				*now_res = (base) buffer;
				buffer >>= BBITS;
			}

			for (now_res = res.bn + size_a + size_b - 1; !(*now_res); --now_res);
			res.en = now_res;

			max1.ba = max2.ba = min.ba = r1.ba = r2.ba = nullptr;
			delete [] tmp;
		}
	} else {
		this -> usual_mul_mem(b, res);
	}
	return;
}

bool BigNumber::miller_rabin_test() {
    if (!(*this > 3)) {
        if (*bn == 1 || *bn == 0) {
            return false;
        }
        else return true;
    }
    if (!(*bn & 1)) {
        return false;
    }

    BigNumber r = *this - 1, b;
    BigNumber decn = r;
    size_t size_p = en - bn + 1, sbits, sbase, s;
    base mask, *now;

    for (now = r.bn; now <= r.en; ++now) {
        mask = 1;
        for (int j = 0; j < BBITS; mask <<= 1, ++j) {
            if (mask & *now) {
                sbits = j;
                sbase = now - r.bn;
                j = BBITS;
                now = r.en + 1;
            }
        }
    }

    r.bn += sbase;
    if (sbits) r >>= sbits;
    s = BBITS * sbase + sbits; //умножаем 32 на количество нулевых баз и прибавляем колечество нулевых бит в первой ненулевой базе
    //s - количество двоек на которые нужно поделить n - 1, для становления его нечётным

    int i;
    for (i = MR_REL_PARAM; i > 0; --i) {
        b = BigNumber::gen_num_less_than(decn);
        while (b < 2) {
            b = BigNumber::gen_num_less_than(decn);
        }
        b = b.pow(r, *this);

        if (b != 1 && b != decn) {
            for (int j = 0; j < s - 1 && b != decn; ++j) {
                b = b.sqr() % *this;
                if (b == 1) return false;
            }
            if (b != decn) return false;
        }
    }
    return true;
}

bool BigNumber::is_prime() {
    return this -> miller_rabin_test();
}

size_t BigNumber::bitsize () {
    size_t bits = (en - bn + 1) * BBITS;
    base mask = ((base)1) << (BBITS - 1);
    for (; !((*en) & mask); mask >>= 1){
        --bits;
    }
    return bits;
}
BigNumber BigNumber::gen_prime (size_t bits) {
    BigNumber b = BigNumber::gen_num_with_bits(bits);
    if (!(*(b.bn) & 1)) {
        b += 1;
    }
    while (true) {
        for (; b.bitsize() == bits && !(b.is_prime()); b += 2);
        if (b.bitsize() == bits) {
            break;
        }
        else b = BigNumber::gen_num_with_bits(bits);
    }
    //for (; !(b.is_prime()); b += 2);
    return b;
}

BigNumber BigNumber::random (int length) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand(ts.tv_nsec);
    base *ba, *ea, *en;
    int num_length = length;
    ba = new base[length];
    if (!ba) throw ALLOC_ERR;

    ea = ba + length - 1;
    base *t = ea;
    en = t;
    while (t > ba && !(*t = (base)rand())) {
        --t;
        --num_length;
    }
    for (; t >= ba; --t) {
        *t = (base)rand();
    }

    return BigNumber (ba, num_length, length);
}

BigNumber BigNumber::gen_num_with_bits (size_t num) {
    size_t num_base = (num / BBITS) + 1, num_bits = num;
    return BigNumber::gen_num_with_bits (num_base, num_bits);
}

BigNumber BigNumber::gen_num_with_bits (size_t num_base, size_t num_bits) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand(ts.tv_nsec);
    base *ba, *en, *bn, *t;
    bn = ba = new base[num_base];
    if (!ba) throw ALLOC_ERR;
    en = ba + num_base - 1;

    if (num_bits >= BBITS) {
        num_bits %= BBITS;
    }
    if (num_bits == 0) {
        if (num_base > 1) {
            --num_base;
            num_bits = BBITS - 1;
        }
        else throw "strange params";
    }

    base mask = (1 << num_bits) - 1;
    *en = rand() & mask;
    mask = 1 << (num_bits - 1);
    *en |= mask;
    for (t = en - 1; t >= bn; --t) {
        *t = rand();
    }

    return BigNumber (ba, num_base, num_base);
}

BigNumber BigNumber::gen_num_less_than (BigNumber& b) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); 
    srand(ts.tv_nsec);
    base *ba, *en, *bn, *t, *bt;
    size_t size_b = b.en - b.bn + 1;
    ba = bn = new base[size_b];
    if (!ba) throw ALLOC_ERR;
    en = bn + size_b - 1;

    if (b == 0) {
        return BigNumber(0);
    }
    for (t = bn; t <= en; ++t) {
        *t = rand();
    }
    if (*en > *(b.en)) {
        *en = *en % *(b.en);
    }
    if (en != 0 || en == bn) {
        if (*en == *(b.en)) {
            for (t = en - 1, bt = b.en - 1; t > ba; --t) {
                if (*t > *bt) {
                    *t = *t % *bt;
                    return BigNumber (ba, size_b, size_b);
                }
                else if (*t < *bt) {
                    return BigNumber (ba, size_b, size_b);
                }
            }
            if (en != bn) {
                return BigNumber (ba, size_b, size_b) - 1;
            }
            else {
                if (*bn) {
                    --(*bn);
                }
                else {
                    return BigNumber (ba, size_b, size_b) - 1;
                }
            }
            return BigNumber (ba, size_b, size_b);
        }
        else {
            return BigNumber (ba, size_b, size_b);
        }
    }

    for (t = en - 1; t > ba && !(*t); --t);
    return BigNumber (ba, t - ba + 1, size_b);
}

bool trail_div(BigNumber_d & div, BigNumber & border) {
    BigNumber *num = this;
    BigNumber *d = nullptr, *tmp;
    size_t size_num = (num -> en) - (num -> bn) + 1;
    BigNumber *q = new BigNumber(size_num, 0), *r = new BigNumber(size_num, 0), *q2 = new BigNumber(size_num, 0);
    auto it = div.begin();
    d = BigNumber::next_d(d);
    for(;;) {
        BigNumber::div_mod(*num, *d, q, r);
        if (r -> en == r -> bn && *(r.bn) == 0) {
            if (div.empty()) {
                div.push_back(new BP(new BigNumber(*d), 1));
            } else {
                it = div.back();
                if (*((it) -> num) == *d) {
                    ++((*it) -> degree);
                } else {
                    div.push_back(new BP(new BigNumber(*d), 1));
                }
            }
            tmp = num;
            num = q;
            q = tmp;

            if ((num -> bn) == (num -> en) && *(num -> bn) == 1) {
                delete r,d;
                if (this == q) {
                    *q = *num;
                    delete num;
                } else delete q;
                return true;
            }
            continue;
        } else {
            if (*q < *d){
                div.push_back(new BP(new BigNumber(*num), 1));
                delete r, d;
                if (this == q) {
                    *q = *num;
                    delete num;
                } else delete q;
                return true;
            }
        }

        q -> clean();
        r -> clean();

        d = BigNumber::next_d(d);
        if (*d > border) {
            div.push_back(new BP(new BigNumber(*num), 1));
            if (this == q) {
                *q = *num;
                delete num;
            } else delete q;
            delete r, d;
            return false;
        }
    }
}

BigNumber * BigNumber::next_d(BigNumber *d) {
    static bool oe = false;
    if(d) {
        if (d -> bn) {
            if ((d -> en) == (d -> bn) && *(d -> bn) < 7) {
                *(d -> bn) += 2;
                return d;
            } else throw "bad d";
        } 
    } else {
        oe = false;
        return new BigNumber(3);
    }
}

void BigNumber::clean() {
    base * tmp = ba;
    for (; tmp <= ea; ++tmp) {
        *tmp = 0;
    }
    bn = en = ba;
}

BigNumber BigNumber::discret_log(BigNumber &g, BigNumber &p) {
    pol_tup slow(1, 0, 0), fast(1, 0, 0);
    BigNumber n = p - 1, tmp(0), rev_tmp(0), r(0), nn(0);
    bool fl;
    BigNumber m = n.sqrt();
    for (;;) {
        f_pollard(slow, *this, g, p, n);
        f_pollard(fast, *this, g, p, n);
        f_pollard(fast, *this, g, p, n);
        if (fast.x == slow.x) {
            if (fast.b > slow.b) {
                fl = true;
                tmp = fast.b - slow.b;
            } else {
                fl = false;
                tmp = slow.b - fast.b;
            }

            r = tmp.gcd(n);
            if (r.en == r.bn && *r.bn == 0) {
                slow.y = BigNumber::gen_num_less_than(n);
                slow.b = BigNumber::gen_num_less_than(n);
                slow.x = g.pow(slow.y, p);
                r = this -> pow(slow.b, p);
                slow.x *= r;
                slow.x = slow.x % p;
                fast.x = slow.x;
                fast.y = slow.y;
                fast.b = slow.b;
                continue;
            }
            if (r > m) {
                 slow.y = BigNumber::gen_num_less_than(n);
                 slow.b = BigNumber::gen_num_less_than(n);
                 slow.x = g.pow(slow.y, p);
                 r = this -> pow(slow.b, p);
                 slow.x *= r;
                 slow.x = slow.x % p;
                 fast.x = slow.x;
                 fast.y = slow.y;
                 fast.b = slow.b;
                 continue;
            }

            if (r.en == r.bn && *r.bn == 1) {
                rev_tmp = tmp.inverse_mod(n);
                if (!fl) {
                    return ((((n + fast.y) - slow.y)) * rev_tmp) % n;
                } else return ((((n + slow.y) - fast.y)) * rev_tmp) % n;
            } else {
                tmp = tmp / r;
                nn = n / r;
                rev_tmp = tmp.inverse_mod(nn);
                 if (!fl) {
                    return ((((n + fast.y) - slow.y)) * rev_tmp) % nn;
                } else return ((((n + slow.y) - fast.y)) * rev_tmp) % nn;

                r = tmp;
                for (;;) {
                    if (g.pow(tmp, p) == *this) {
                        return tmp;
                    }
                    tmp += nn;
                    if (tmp > n) {
                        tmp = tmp % n;
                    }
                    if (tmp == r) {
                        throw "nope";
                    }
                }
            }
        }
    }
}

void BigNumber::f_pollard(pol_tup & tup, BigNumber & a, BigNumber & g, BigNumber & p, BigNumber & n) {
    base c = tup.x % 3;
    if (c == 1) {
        tup.x = (tup.x * a) % p;
        tup.b = (tup.b + 1) % n;
    } else if (c == 2) {
        tup.x = tup.x.sqr() % p;
        tup.y = (tup.y << 1) % n;
        tup.b = (tup.b << 1) % n;
    } else {
        tup.x = (tup.x * g) % p;
        tup.y = (tup.y + 1) % n;
    }
}

BigNumber BigNumber::gcd(BigNumber & n) {
    base *tmp;
    BigNumber a(*this), b(n);
    if (b > a) {
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
    BigNumber r = a % b;
    for (; r.en == r.bn && *r.bn == 1;) {
        a = b;
        b = r;
        r = a % b;
    }
    return b;
}

BigNumber BigNumber::inverse_mod(BigNumber & mod) {
    base * tmp;
    BigNumber a(mod), x1(0), x2(1), y1(1), y(0), tmp1(1), q(en - bn + 1, 0), r(en - bn + 1, 0);
    BigNumber b = *this % mod;
    BigNumber::div_mod(a, b, &q, &r);

    for (;!( r.en == r.bn && *r.bn == 0);) {
        tmp1 = (q * y2) % mod;
        if (tmp1 > y1) {
            tmp1 = (mod + y1) - tmp1; 
        } else tmp1 = y1 - tmp1;
        y1 = y2;
        y2 = tmp1;
        tmp1 = (q * x2) % mod;
        if (tmp1 > x1) {
            tmp1 = (mod + x1) - tmp1;
        } else tmp1 = x1 - tmp1;
        x1 = x2;
        x2 = tmp1;
        a = b;
        b = r;
        BigNumber::div_mod(a, b, &q, &r);
    }

    if (!(b.bn == b.en && *b.bn == 1)) {
        throw "bad mod";
    }
    return y2;
}

void BigNumber::sqrt(BigNumber &res, BigNumber &q) {
    if (!q) throw "bad q";

    size_t num_size = en - bn + 1;
    size_t res_sezr = (num_size >> 1) + 2;
    BigNumber::resize(res, res_size);
    q -> clean();
    q -> bn[res_size - 1] = 1;
    q -> en = q -> bn + res_size - 1;

    do {
        res.light_copy(*q);
        BigNumber::div_mod(*this, res, q, nullptr);
        *q += res;
        *q >>= 1;
    } while (res > *q);

    return;
}

BigNumber BigNumber::sqrt() {
    size_t num_size = en - bn + 1;
    size_t a_size = (num_size >> 1) + 2;
    BigNumber a(a_size, 0);
    BigNumber *q = new BigNumber(a_size, 0);
    q -> bn[a_size - 1] = 1;
    q -> en = q -> bn + a_size - 1;

    do {
        a.light_copy(*q);
        BigNumber::div_mod(*this, a, q, nullptr);
        *q += a;
        *q >>= 1;
    } while (a > *q);

    delete q;

    return a;
}

void BigNumber::light_copy(BigNumber & b) {
    base *tmp1, tmp2;
    
    for (tmp1 = ba; tmp1 <= ea; ++tmp1) {
        tmp1 = 0;
    }

    for (tmp1 = ba, tmp2 = b.bn; tmp1 <= ea && tmp2 = b.en; ++tmp1, ++tmp2) {
        *tmp1 = *tmp2;
    }

    bn = ba;

    if (tmp1 > ea && tmp2 <= b.en) {
        throw "not light"
    }

    en = tmp - 1;
}

BigNumber BigNumber::pow(base n) {
    BigNumber a(*this), res(1);
    
    for (;n; n >>= 1) {
        if (n & 1) {
            res *= a;
        }
        a = a.sqr();
    }
    return res;
}

BigNumber BigNumber::pow(base b, BigNumber & b) {
    BigNumber a(*this), res(1);
    base tmp;

    for (;n; n >>= 1) {
        if (n & 1) {
            res *= a;
            if (res > b) {
                return res;
            }
        }
        a = a.sqr();
    }
    return res;
}

bool BigNumber::ro_pollard(BigNumber_d & div) {
    BigNumber a(2), b(2), c(1), d(1);
    for (;;) {
        a = a.sqr();
        a += c;
        a = a % *this;
        b = b.sqr();
        b += c;
        b = b % *this;
        b = b.sqr();
        b += c;
        b = b % *this;
        if (b == a) return false;
        if (a < b) {
            d = (b - a).gcd(*this);
        } else {
            d = (a - b).gcd(*this);
        }
        if (!(d.en == d.bn && *d.bn == 1)) {
            div.push_back(new BP(new BigNumber(), 1));
            d = *this / d;
            div.push_back(new BP(new BigNumber(d), 1));
            return true;
        }
    }
}

bool BigNumber::p_1_pollard(BigNumber_d & div, base B, base lim) {
    std::vector<base> primes;
    BigNumber n_m(*this);
    BigNumber a(en - bn + 1, 0);
    n_m -= 1;
    BigNumber d;
    BigNumber q(en - bn + 1, 0);
    base l;
    size_t size_n = en - bn + 1, size_e;
    base log_2_n;
    base shft = 31, mask = ((base)1) << (BBITS - 1);

    generate_base_less_border(primes, B);

    for (; !(mask & *en); --shft, mask >>= 1);
    log_2_n = size_n * BBITS + shft;

    for(;;) {
        a = BigNumber::gen_num_less_than(n_m);
        for (auto i = primes.begin(); i != primes.end(); ++i) {
            shft = 31, mask = ((base)1) << (BBITS - 1);
            for (; !(mask & *i); --shft, mask >>= 1);
            l = log_2_n / shft + 1;
            q.clean();
            *q.bn  = *i;
            q = q.pow(l);
            a = a.pow(q, *this);
        }

        if (a.en == a.bn && *a.bn == 1) continue;

        d = (a - 1).gcd(*this);

        if ((d.bn != d.en || *d.bn > 1) && d < *this) {
            BigNumber q = *this / d;
            div.push_back(new BP(new BigNumber(q), 1));
            div.push_back(new BP(new BigNumber(d), 1));
            break;
        } else {
            if (lim == 0) {
                return false;
            } else {
                --lim;
            }
            l = B + B_STEP;
            if (l > B) {
                B = l;
            } else return false;

            generate_base_less_border(primes, B);
        }
    }
    return true;
}

bool BigNumber::ferma_with_shft(BigNumber_d &div) {
    base r = FERMA_BASE;
    std::vector<base> primes = generate_base(r);
    std::vector<base> ki;
    std::vector<uint8_t *> s;
    base tmp, jac;
    uint8_t *t;

    for (auto i = primes.begin(); i != primes.end(); ++i) {
        tmp = *this % *i;
        tmp = *i - tmp;
        t = new uint8_t[*i];
        for (dbase j = 0; j < *i; ++j) {
            jac = (j * j + tmp) % *i;
            if (-1 != jac_simb(jac, *i)) {
                t[j] = 1;
            } else {
                t[j] = 0;
            }
        }
        s.push_back(t);
    }

    BigNumber x = this -> sqrt(), z(0), y(0);

    if (x.sqr() == *this) {
        div.push_back(new BP(new BigNumber(x), 2));
        return false;
    }

    x += 1;
    BigNumber tall = (*this + 1) >> 1;
    for (auto i = primes.begin(); i != primes.end(); ++i) {
        ki.push_back(x % *i);
    }
    for (;;) {
        base l = 0;
        bool fl = true;
        for (auto i = ki.begin(); i != ki.end(); ++i, ++l) {
            if (s[l][*i] == 0) {
                fl = false;
                break;
            }
        }
        if (fl) {
            z = x.sqr() - *this;
            y = z.sqrt();
            if (y.sqr() == z) {
                z = x + y;
            }
        }
    }
}

void BigNumber::generate_base_less_border(std::vector<base> & primes, base B) {
    bool fl = false;
    base l;

    if (primes.empty()) {
        primes.push_back(2);
        l = 3;
    } else {
        l = primes.back();
        if (!(l & 1)) {
            l = 3;
        }
    }

    for (base c = l; c < B; c += 2) {
        fl = true;
        for (auto i = primes.begin(); i < primes.end(); ++i) {
            if ((c % *i) == 0) {
                fl = false;
                break;
            }
        }
        if (fl) {
            primes.push_back(c);
            fl = false;
        }
    }
}

std::vector<base> BigNumber::generate_base(base k) {
    bool fl = false;
    std::vector<base> primes;
    primes.push_back(2);
    base count;

    for (base c = 3, count = 1; count < k; c += 2) {
        fl = true;
        for (auto i = primes.begin(); i != primes.end(); ++i) {
            if ((c % *i) == 0) {
                fl = false;
                break;
            }
        }
        if (fl) {
            primes.push_back(c);
            count++;
            fl = false;
        }
    }
    return primes;
}

BigNumber_d BigNumber::factor() {
    BigNumber_d div;
    
    if (en == bn) {
        if (*bn == 0) {
            throw "div zero";
        } else if (*bn == 1) {
            return div;
        }
    }

    if (this -> is_prime()) {
        div.push_back(new BP(new BigNumber(*this), 1));
        return div;
    }

    BigNumber a(*this);
    if (!((*(a.bn) & 1))) {
        uint shft = 0;
        for (;*(a.bn) == 0 && a.bn < a.en; shft  += BBITS, ++(a.bn));
        base tmp = *(a.bn);
        for (; (tmp & 1) == 0; ++shft, tmp >>= 1);
        a >>= (shft & (BBITS - 1));
        div.push_back(new BP(new BigNumber(2), shft));
    }

    if (a.en == a.bn && *(a.bn) == 1) {
        return div;
    }

    BigNumber *border;
    border = new BigNumber(TRIAL_BORDER);

    if (a.trial_div(div, *border)) {
        delete border;
        return div;
    }

    BP * b = div.back();
    BigNumber *num = b -> num;

    if (num -> is_prime()) {
        delete border;
        return div;
    }

    div.pop_back();
    b -> num = nullptr;
    delete b;
    std::queue<BP *> q;
    BigNumber_d div1;

    for (; !q.empty();) {
        b = q.front();
        q.pop();
        div1.clear();
        if (!b -> num -> p_1_pollard(div1, P_M_BORDER, 10) && !b -> num -> ro_pollard(div1)) {
            b -> num -> ferma_with_shft(div1);
        }
        for (auto i = div1.begin(); i != div1.end();) {
            if ((*i) -> num -> is_prime()) {
                div.push_back(*i);
            } else {
                q.push(*i);
            }
            i = div1.erase(i);
            if (i == div1.end()) {
                break;
            }
        }
    }
    for (auto i = div.begin(); i != div.end(); ++i) {
        auto j = i;
        ++j;
        for (; j != div.end();) {
            if (((*j) -> num) == ((*i) -> num)) {
                (*i) -> degree += (*j) -> degree;
                j = div.erase(j);
                if (j == div.end()) {
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
