#pragma once
#include <iostream>

#include "util.h"


class TestUtil
{
public:
    static void run_all_tests()
    {
        std::cout << "[🚀 Running all tests...]" << std::endl;
        int total_tests = 0;
        int passed_tests = 0;
        
        passed_tests += test_add();
        passed_tests += test_multiply();
        passed_tests += test_greet();
        passed_tests += test_square_vector();
        passed_tests += test_calculator();
        total_tests = 5;
        
        if (passed_tests == total_tests) {
            std::cout << "[🎉 Test Summary]: " << passed_tests << "/" << total_tests << " tests passed. ALL TESTS PASSED! ✅" << std::endl;
        } else {
            std::cout << "[⚠️ Test Summary]: " << passed_tests << "/" << total_tests << " tests passed. Some tests failed! ❌" << std::endl;
        }
    }

private:
    // Test add function
    static int test_add()
    {
        std::cout << "➕ Testing add function..." << std::endl;
        bool all_passed = true;
        
        if (add(2, 3) != 5) {
            std::cout << "  ❌ FAILED: add(2, 3) expected 5, got " << add(2, 3) << std::endl;
            all_passed = false;
        }
        if (add(-1, 1) != 0) {
            std::cout << "  ❌ FAILED: add(-1, 1) expected 0, got " << add(-1, 1) << std::endl;
            all_passed = false;
        }
        if (add(0, 0) != 0) {
            std::cout << "  ❌ FAILED: add(0, 0) expected 0, got " << add(0, 0) << std::endl;
            all_passed = false;
        }
        if (add(-5, -3) != -8) {
            std::cout << "  ❌ FAILED: add(-5, -3) expected -8, got " << add(-5, -3) << std::endl;
            all_passed = false;
        }
        
        if (all_passed) {
            std::cout << "  ✅ PASSED: add function tests" << std::endl;
            return 1;
        } else {
            std::cout << "  ❌ FAILED: add function tests" << std::endl;
            return 0;
        }
    }

    // Test multiply function
    static int test_multiply()
    {
        std::cout << "✖️ Testing multiply function..." << std::endl;
        bool all_passed = true;
        
        if (multiply(2.0, 3.0) != 6.0) {
            std::cout << "  ❌ FAILED: multiply(2.0, 3.0) expected 6.0, got " << multiply(2.0, 3.0) << std::endl;
            all_passed = false;
        }
        if (multiply(-2.0, 3.0) != -6.0) {
            std::cout << "  ❌ FAILED: multiply(-2.0, 3.0) expected -6.0, got " << multiply(-2.0, 3.0) << std::endl;
            all_passed = false;
        }
        if (multiply(0.0, 5.0) != 0.0) {
            std::cout << "  ❌ FAILED: multiply(0.0, 5.0) expected 0.0, got " << multiply(0.0, 5.0) << std::endl;
            all_passed = false;
        }
        if (multiply(2.5, 4.0) != 10.0) {
            std::cout << "  ❌ FAILED: multiply(2.5, 4.0) expected 10.0, got " << multiply(2.5, 4.0) << std::endl;
            all_passed = false;
        }
        
        if (all_passed) {
            std::cout << "  ✅ PASSED: multiply function tests" << std::endl;
            return 1;
        } else {
            std::cout << "  ❌ FAILED: multiply function tests" << std::endl;
            return 0;
        }
    }

    // Test greet function
    static int test_greet()
    {
        std::cout << "👋 Testing greet function..." << std::endl;
        bool all_passed = true;
        
        if (greet("World") != "Hello, World!") {
            std::cout << "  ❌ FAILED: greet(\"World\") expected \"Hello, World!\", got \"" << greet("World") << "\"" << std::endl;
            all_passed = false;
        }
        if (greet("Alice") != "Hello, Alice!") {
            std::cout << "  ❌ FAILED: greet(\"Alice\") expected \"Hello, Alice!\", got \"" << greet("Alice") << "\"" << std::endl;
            all_passed = false;
        }
        if (greet("") != "Hello, !") {
            std::cout << "  ❌ FAILED: greet(\"\") expected \"Hello, !\", got \"" << greet("") << "\"" << std::endl;
            all_passed = false;
        }
        
        if (all_passed) {
            std::cout << "  ✅ PASSED: greet function tests" << std::endl;
            return 1;
        } else {
            std::cout << "  ❌ FAILED: greet function tests" << std::endl;
            return 0;
        }
    }

    // Test square_vector function
    static int test_square_vector()
    {
        std::cout << "📊 Testing square_vector function..." << std::endl;
        bool all_passed = true;

        // Test with normal values
        std::vector<int> input1 = {1, 2, 3, 4};
        std::vector<int> expected1 = {1, 4, 9, 16};
        std::vector<int> result1 = square_vector(input1);
        if (result1 != expected1) {
            std::cout << "  ❌ FAILED: square_vector normal values test" << std::endl;
            all_passed = false;
        }

        // Test with negative values
        std::vector<int> input2 = {-2, -3, 0, 5};
        std::vector<int> expected2 = {4, 9, 0, 25};
        std::vector<int> result2 = square_vector(input2);
        if (result2 != expected2) {
            std::cout << "  ❌ FAILED: square_vector negative values test" << std::endl;
            all_passed = false;
        }

        // Test with empty vector
        std::vector<int> input3 = {};
        std::vector<int> expected3 = {};
        std::vector<int> result3 = square_vector(input3);
        if (result3 != expected3) {
            std::cout << "  ❌ FAILED: square_vector empty vector test" << std::endl;
            all_passed = false;
        }

        if (all_passed) {
            std::cout << "  ✅ PASSED: square_vector function tests" << std::endl;
            return 1;
        } else {
            std::cout << "  ❌ FAILED: square_vector function tests" << std::endl;
            return 0;
        }
    }

    // Test Calculator class
    static int test_calculator()
    {
        std::cout << "🧮 Testing Calculator class..." << std::endl;
        bool all_passed = true;

        // Test initialization
        Calculator calc(10.0);
        if (calc.get_value() != 10.0) {
            std::cout << "  ❌ FAILED: Calculator initialization expected 10.0, got " << calc.get_value() << std::endl;
            all_passed = false;
        }

        // Test add
        calc.add(5.0);
        if (calc.get_value() != 15.0) {
            std::cout << "  ❌ FAILED: Calculator add expected 15.0, got " << calc.get_value() << std::endl;
            all_passed = false;
        }

        // Test subtract
        calc.subtract(3.0);
        if (calc.get_value() != 12.0) {
            std::cout << "  ❌ FAILED: Calculator subtract expected 12.0, got " << calc.get_value() << std::endl;
            all_passed = false;
        }

        // Test reset
        calc.reset();
        if (calc.get_value() != 0.0) {
            std::cout << "  ❌ FAILED: Calculator reset expected 0.0, got " << calc.get_value() << std::endl;
            all_passed = false;
        }

        // Test with negative numbers
        calc.add(-5.0);
        if (calc.get_value() != -5.0) {
            std::cout << "  ❌ FAILED: Calculator add negative expected -5.0, got " << calc.get_value() << std::endl;
            all_passed = false;
        }

        calc.subtract(-3.0); // subtracting negative = adding positive
        if (calc.get_value() != -2.0) {
            std::cout << "  ❌ FAILED: Calculator subtract negative expected -2.0, got " << calc.get_value() << std::endl;
            all_passed = false;
        }

        if (all_passed) {
            std::cout << "  ✅ PASSED: Calculator class tests" << std::endl;
            return 1;
        } else {
            std::cout << "  ❌ FAILED: Calculator class tests" << std::endl;
            return 0;
        }
    }
};
