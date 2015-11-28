#include <iostream>
#include <big_num.h>


BigNumber::BigNumber() {
    bn = en = ea = ba = nullptr;
}

BigNumber::~BigNumber() {
    if (ba) {
        delete [] ba;
    }
    bn = en = ea = ba = nullptr;
}

BigNumber::BigNumber(int len, base fill) {
    ba = new base[len];
    bn = ba;
    ea = ba + len - 1;
    if (fill == 0) {
        en = bn;
    }
    else {
        en = ea;
    }
    for (base *tmp = ba; tmp <= ea; ++tmp) {
        *tmp = fill;
    }
}

BigNumber::BigNumber(base b) {
    ba = new base[1];
    bn = en = ea = ba;
    *bn = b;
}

BigNumber::BigNumber(const BigNumber& b) {
    if (b.ba == nullptr) {
        bn = en = ea = ba = nullptr;
        return;
    }
    size_t len = b.en - b.bn + 1;
    ba = new base[len];
    bn = ba;
    ea = en = ba + len - 1;
    for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 <= en; ++tmp1, ++tmp2) {
        *tmp1 = *tmp2;
    }
}

BigNumber::operator = (const BigNumber& b) {
    if (this == &b) return;
    if (b.ba == nullptr) {
        bn = en = ea = ba = nullptr;
        return;
    }
    size_t len = b.en - b.bn + 1;
    ba = new base[len];
    bn = ba;
    ea = en = ba + len - 1;
    for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 <= en; ++tmp1, ++tmp2) {
        *tmp1 = *tmp2;
    }

}

BigNumber::Compare(const BigNumber& b) {
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

BigNumber BigNumber::operator + (const BigNumber& b) {
    size_t size_max;
    size_t size_min;
    int i;
    base *min, *max;
    if (en - bn >= b.en - b.bn) {
        size_max = en - bn + 1;
        size_min = b.en - b.en + 1;
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

BigNumber BigNumber::operator - (const BigNumber& b) {
    if (*this < b) throw "< 0";
    size_t size_a = en - bn + 1;
    size_t size_b = b.en - b.bn + 1;
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
