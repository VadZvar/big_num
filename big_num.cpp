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
    if ((bn - en) == (b.bn - b.en)) {
        for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 >= en; --tmp1, --tmp2) {
            if (*tmp1 > *tmp2) return 1;
            if (*tmp1 < *tmp2) return -1;
    }
        return 0;
}
    else {
        if ((bn - en) > (b.bn - b.en)) return 1;
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
