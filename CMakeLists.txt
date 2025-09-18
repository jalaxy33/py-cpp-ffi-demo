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

    set(TARGET_NAME cpp_app)
    add_executable(${TARGET_NAME} src/main.cpp src/util.cpp)
    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/test)
endif()

