cmake_minimum_required(VERSION 3.15)

if (BUILD_EXTENSION)
    message(STATUS "Building C++ extension for python.")

    project(${SKBUILD_PROJECT_NAME} VERSION ${SKBUILD_PROJECT_VERSION} LANGUAGES CXX)

    # check if pybind11 is available
    set(PYBIND11_FINDPYTHON ON)
    find_package(pybind11 CONFIG REQUIRED)
    message(STATUS "Found pybind11: ${pybind11_VERSION}")

    # create extension module
    set(MODULE_NAME "_core")
    pybind11_add_module(${MODULE_NAME} MODULE src/extension.cpp src/util.cpp)
    target_include_directories(${MODULE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    # install module
    install(TARGETS ${MODULE_NAME} DESTINATION ${SKBUILD_PROJECT_NAME})

    # generate stub files
    add_custom_command(TARGET ${MODULE_NAME}
        POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} -m pybind11_stubgen ${MODULE_NAME} -o ${CMAKE_CURRENT_BINARY_DIR}/${SKBUILD_PROJECT_NAME}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${MODULE_NAME}>
        COMMENT "Generating stub files for ${SKBUILD_PROJECT_NAME} module"
        VERBATIM
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${SKBUILD_PROJECT_NAME}/${MODULE_NAME}.pyi
        DESTINATION ${SKBUILD_PROJECT_NAME}
        OPTIONAL
    )

else()
    message(STATUS "Building pure C++ project.")

    set(PROJECT_NAME "py-cpp-ffi")
    project(${PROJECT_NAME} LANGUAGES CXX)

    # --------------------------------------------------------------------------------
    # Build standalone executable
    # --------------------------------------------------------------------------------
    set(TARGET1 cpp_app)
    add_executable(${TARGET1} src/main.cpp src/util.cpp)
    target_include_directories(${TARGET1} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_include_directories(${TARGET1} PRIVATE ${CMAKE_SOURCE_DIR}/test)

    # --------------------------------------------------------------------------------
    # Call python from C++
    # --------------------------------------------------------------------------------

    # setup python environment
    set(PYTHON_VENV_DIR ${CMAKE_SOURCE_DIR}/.venv)

    if (WIN32)
        set(PYTHON_EXECUTABLE ${PYTHON_VENV_DIR}/Scripts/python.exe)
    else()
        set(PYTHON_EXECUTABLE ${PYTHON_VENV_DIR}/bin/python)
    endif()
    set(Python_EXECUTABLE ${PYTHON_EXECUTABLE})  # for compatibility with pybind11

    if (NOT EXISTS ${PYTHON_EXECUTABLE})
        message(FATAL_ERROR "Python executable not found at ${PYTHON_EXECUTABLE}. Please create a virtual environment at ${PYTHON_VENV_DIR} and install the required packages.")
    endif()

    # set pybind11 dependency
    set(pybind11_ROOT ${PYTHON_VENV_DIR}/Lib/site-packages/pybind11)
    set(pybind11_DIR ${pybind11_ROOT}/share/cmake/pybind11)

    if (NOT EXISTS ${pybind11_ROOT})
        message(FATAL_ERROR "pybind11 not found at ${pybind11_ROOT}. Please install pybind11 in the virtual environment.")
    endif()

    # set pybind11 dependency
    set(PYBIND11_FINDPYTHON ON)
    find_package(pybind11 CONFIG REQUIRED)

    # build executable
    set(TARGET2 call_py)
    add_executable(${TARGET2} src/main_call_py.cpp)
    target_link_libraries(${TARGET2} PRIVATE pybind11::embed)
    target_include_directories(${TARGET2} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test)


    # get python info
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_config_var('installed_platbase'))"
        OUTPUT_VARIABLE PYTHON_HOME_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_config_var('py_version_nodot'))"
        OUTPUT_VARIABLE PYTHON_VERSION_NODOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Windows-specific configuration: find Python DLL and PYTHONHOME
    if(WIN32)
        set(PYTHON_DLL_PATH "${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NODOT}.dll")

        # Copy Python DLL to output directory
        if(EXISTS ${PYTHON_DLL_PATH})
            add_custom_command(
                TARGET ${TARGET2}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${PYTHON_DLL_PATH}
                    $<TARGET_FILE_DIR:${TARGET2}>
                COMMENT "Copying Python DLL to output directory"
            )
        else()
            message(WARNING "Python DLL not found at: ${PYTHON_DLL_PATH}")
        endif()

        
    endif()
        
    # Ensure correct runtime python environment (Set PYTHONHOME and PYTHONPATH)

    string(REPLACE "\\" "\\\\" PYTHON_HOME_PATH_ESCAPED "${PYTHON_HOME_PATH}")
    target_compile_definitions(${TARGET2} PRIVATE PYTHON_HOME_PATH="${PYTHON_HOME_PATH_ESCAPED}")
    
    # Define virtual environment site-packages path
    set(VENV_SITE_PACKAGES_PATH "${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages")
    string(REPLACE "\\" "\\\\" VENV_SITE_PACKAGES_ESCAPED "${VENV_SITE_PACKAGES_PATH}")
    target_compile_definitions(${TARGET2} PRIVATE VENV_SITE_PACKAGES_PATH="${VENV_SITE_PACKAGES_ESCAPED}")

    # Create a custom target to run with correct environment
    if (EXISTS ${PYTHON_HOME_PATH})
        message(STATUS "Python HOME: ${PYTHON_HOME_PATH}")

        # Setup PYTHONPATH to include virtual environment site-packages
        set(VENV_SITE_PACKAGES_PATH "${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages")
        if(WIN32)
            # On Windows, use semicolon as path separator
            set(PYTHONPATH_VALUE "${VENV_SITE_PACKAGES_PATH};${CMAKE_SOURCE_DIR}/src")
        else()
            # On Unix systems, use colon as path separator
            set(PYTHONPATH_VALUE "${VENV_SITE_PACKAGES_PATH}:${CMAKE_SOURCE_DIR}/src")
        endif()

        add_custom_target(set_python_env
            COMMAND ${CMAKE_COMMAND} -E env 
                "PYTHONHOME=${PYTHON_HOME_PATH}" 
                "PYTHONPATH=${PYTHONPATH_VALUE}"
                $<TARGET_FILE:${TARGET2}>
            DEPENDS ${TARGET2}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running with proper Python environment and PYTHONPATH"
        )
    else()
        message(FATAL_ERROR "Python HOME directory not found at: ${PYTHON_HOME_PATH}")
    endif()

endif()

