#include "util.h"

#include "test_util.h"

int main()
{
    print_hello();
    
    TestUtil::run_all_tests();
    
    return 0;
}
