/*
1.3 
Если в числе содержится только один бит со значением 1, 
записать в выходной поток OK. 
Иначе записать FAIL
*/ 

#include <iostream>
#include <cassert>
#include <climits>

#define assertm(exp, msg) assert(((void)msg, exp))

bool check(unsigned int n) {
    int bit_sum = 0;

    while (n > 0) {
        if (n & 1 == 1)
            bit_sum++;
        if (bit_sum > 1)
            // Если битов = 1 больше одного, нет смысла дальше идти по числу
            return false;
        n = n>>1;
    }

    return bit_sum == 1;
}

int main() {
    unsigned long int number;
    std::cin >> number;

    assertm(number >= 0, "Number must be greater than or equal 0");
    assertm(number <= ULONG_MAX, "Number must be lower than or equal to 2**32-1");

    if (check(number))
        std::cout << "OK" << std::endl;
    else
        std::cout << "FAIL" << std::endl;

    return 0;
}
