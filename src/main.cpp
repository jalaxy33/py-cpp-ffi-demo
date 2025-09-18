#include <iostream>

#include "util.h"
#include "test_util.h"

int main()
{
    std::cout << "\n======================= 🚀 Start of main.cpp ====================" << std::endl;

    print_hello();

    TestUtil::run_all_tests();

    std::cout << "======================= 🚀 End of main.cpp ====================" << std::endl;

    return 0;
}
