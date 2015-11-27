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
    for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 <= en; ++tmp1, ++tmp2){
        *tmp1 = *tmp2;
    }
}

BigNumber::operator=(const BigNumber& b) {
    if (this == &b) return;
    if (b.ba == nullptr) {
        bn = en = ea = ba = nullptr;
        return;
    }
    size_t len = b.en - b.bn + 1;
    ba = new base[len];
    bn = ba;
    ea = en = ba + len - 1;
    for (base *tmp1 = bn, *tmp2 = b.bn; tmp1 <= en; ++tmp1, ++tmp2){
        *tmp1 = *tmp2;
    }

}
