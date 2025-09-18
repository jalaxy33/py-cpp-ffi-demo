#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "python_helper.h"
#include "test_call_py.h"

namespace py = pybind11;

int main()
{
    std::cout << "\n======================= 🚀 Start of main_call_py.cpp ====================" << std::endl;

    std::cout << "Calling Python from C++ using pybind11!" << std::endl;

    try {
        // Run all Python integration tests
        TestCallPy::run_all_tests();
        
        std::cout << "\n✅ Python integration testing completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ Python integration testing failed with exception: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n======================= 🚀 End of main_call_py.cpp ====================" << std::endl;
    return 0;
}
