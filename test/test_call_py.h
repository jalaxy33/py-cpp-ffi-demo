#pragma once
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <string>

#include "python_helper.h"

namespace py = pybind11;

class TestCallPy {
public:
    static void run_all_tests() {
        std::cout << "\n[🚀 Running Python integration tests...]" << std::endl;
        
        // setup Python environment
        configurePythonHome();
        py::scoped_interpreter guard{};
        setupVirtualEnvironmentPaths();
        setup_python_path();   // solve relative import issues

        // Run all tests
        int passed = 0;
        passed += test_basic_execution();
        passed += test_numpy_functionality();
        passed += test_data_exchange();
        passed += test_util_functions();
        passed += test_util_class();
        passed += test_module_import();
        
        const int total = 6;
        if (passed == total) {
            std::cout << "[🎉 ALL TESTS PASSED]: " << passed << "/" << total << " ✅" << std::endl;
        } else {
            std::cout << "[⚠️ SOME TESTS FAILED]: " << passed << "/" << total << " ❌" << std::endl;
        }
    }

private:
    // Setup Python path for util module
    static void setup_python_path() {
        py::exec(R"(
            import sys, os
            project_root = os.path.dirname(os.path.dirname(os.getcwd()))
            src_path = os.path.join(project_root, 'src')
            if src_path not in sys.path:
                sys.path.insert(0, src_path)
        )");
    }

    // Test basic Python execution
    static int test_basic_execution() {
        std::cout << "� Testing basic Python execution..." << std::endl;
        try {
            py::print("Python print API works!");
            py::exec("import sys; assert len(sys.version) > 0");
            std::cout << "  ✅ PASSED" << std::endl;
            return 1;
        } catch (const std::exception& e) {
            std::cout << "  ❌ FAILED: " << e.what() << std::endl;
            return 0;
        }
    }

    // Test NumPy functionality
    static int test_numpy_functionality() {
        std::cout << "🔢 Testing NumPy functionality..." << std::endl;
        try {
            py::dict locals;
            py::exec(R"(
                import numpy as np
                arr = np.array([1, 2, 3, 4, 5])
                result = {'sum': int(arr.sum()), 'mean': float(arr.mean())}
                assert result['sum'] == 15 and result['mean'] == 3.0
            )", py::globals(), locals);
            
            std::cout << "  ✅ PASSED" << std::endl;
            return 1;
        } catch (const std::exception& e) {
            std::cout << "  ❌ FAILED: " << e.what() << std::endl;
            return 0;
        }
    }

    // Test C++/Python data exchange
    static int test_data_exchange() {
        std::cout << "🔄 Testing C++/Python data exchange..." << std::endl;
        try {
            std::vector<double> cpp_data = {1.0, 2.0, 3.0};
            py::dict locals;
            locals["cpp_data"] = py::cast(cpp_data);
            
            py::exec(R"(
                import numpy as np
                np_array = np.array(cpp_data)
                result = {'sum': float(np_array.sum()), 'doubled': (np_array * 2).tolist()}
                assert abs(result['sum'] - 6.0) < 0.001
            )", py::globals(), locals);
            
            py::dict result = locals["result"].cast<py::dict>();
            std::vector<double> doubled = result["doubled"].cast<std::vector<double>>();
            
            if (doubled.size() == 3 && doubled[0] == 2.0) {
                std::cout << "  ✅ PASSED" << std::endl;
                return 1;
            }
            return 0;
        } catch (const std::exception& e) {
            std::cout << "  ❌ FAILED: " << e.what() << std::endl;
            return 0;
        }
    }

    // Test util.py functions
    static int test_util_functions() {
        std::cout << "➕ Testing util.py functions..." << std::endl;
        try {
            py::dict locals;
            py::exec(R"(
                from util import py_add, py_multiply, py_greet
                
                # Test functions
                add_result = py_add(5, 3)
                mult_result = py_multiply(2.5, 4.0)
                greet_result = py_greet("World")
                
                # Validate results
                assert add_result == 8
                assert abs(mult_result - 10.0) < 0.001
                assert greet_result == "Hello, World!"
                
                results = {
                    'add': add_result,
                    'multiply': mult_result,
                    'greet': greet_result
                }
            )", py::globals(), locals);
            
            py::dict results = locals["results"].cast<py::dict>();
            int add_val = results["add"].cast<int>();
            std::string greet_val = results["greet"].cast<std::string>();
            
            if (add_val == 8 && greet_val == "Hello, World!") {
                std::cout << "  ✅ PASSED: py_add(5,3)=" << add_val << ", greet=" << greet_val << std::endl;
                return 1;
            }
            return 0;
        } catch (const std::exception& e) {
            std::cout << "  ❌ FAILED: " << e.what() << std::endl;
            return 0;
        }
    }

    // Test util.py PyCalculator class
    static int test_util_class() {
        std::cout << "🧮 Testing PyCalculator class..." << std::endl;
        try {
            py::dict locals;
            py::exec(R"(
                from util import PyCalculator
                
                # Test calculator operations
                calc = PyCalculator(10.0)
                calc.add(5.0)       # 10 + 5 = 15
                calc.multiply(2.0)  # 15 * 2 = 30
                final_value = calc.get_value()
                
                assert abs(final_value - 30.0) < 0.001
                result = {'final_value': final_value}
            )", py::globals(), locals);
            
            py::dict result = locals["result"].cast<py::dict>();
            double final_val = result["final_value"].cast<double>();
            
            if (abs(final_val - 30.0) < 0.001) {
                std::cout << "  ✅ PASSED: Calculator result = " << final_val << std::endl;
                return 1;
            }
            return 0;
        } catch (const std::exception& e) {
            std::cout << "  ❌ FAILED: " << e.what() << std::endl;
            return 0;
        }
    }

    // Test py::module_::import usage
    static int test_module_import() {
        std::cout << "📥 Testing py::module_::import..." << std::endl;
        try {
            // Import modules using module_::import
            py::module_ sys_mod = py::module_::import("sys");
            py::module_ util_mod = py::module_::import("util");
            
            // Test function call via module import
            py::object py_add_func = util_mod.attr("py_add");
            int result = py_add_func(15, 25).cast<int>();
            
            // Test class instantiation via module import
            py::object calc_class = util_mod.attr("PyCalculator");
            py::object calc_instance = calc_class(100.0);
            calc_instance.attr("multiply")(2.0);
            double calc_result = calc_instance.attr("get_value")().cast<double>();
            
            if (result == 40 && abs(calc_result - 200.0) < 0.001) {
                std::cout << "  ✅ PASSED: py_add=" << result << ", calc=" << calc_result << std::endl;
                return 1;
            }
            return 0;
        } catch (const std::exception& e) {
            std::cout << "  ❌ FAILED: " << e.what() << std::endl;
            return 0;
        }
    }
};


