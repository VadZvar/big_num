#include <iostream>
#include "bignum.h"

int main(int argc, char * argv[]){
	BigInt a =  BigInt::gen_num_with_bits(100);
	a.factor();
}
