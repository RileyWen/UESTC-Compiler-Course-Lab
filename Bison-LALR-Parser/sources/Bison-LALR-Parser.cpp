#include <iostream>
#include "driver.hpp"

// run with argv[1] = '../test-input/test.c'

int main(int argc, char *argv[]) {
    int res = 0;
    driver drv;
    drv.trace_parsing = true;
    drv.trace_scanning = true;
    std::cout << argv[1] << "\n";
    if (!drv.parse(argv[1]))
        drv.result->print(0);
    else
        res = 1;
    return res;
}
