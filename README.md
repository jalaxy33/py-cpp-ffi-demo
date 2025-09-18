# py-cpp-ffi-demo

A demo repository for Python-C++ interoperability using pybind11 and scikit-build.

## Features

- Seamless Python-C++ interoperability using [pybind11](https://github.com/pybind/pybind11).
- Automatically generate `__init__.py` and `*.pyi` stub files. (check [pyproject.toml](./pyproject.toml) and [CMakeLists.txt](./CMakeLists.txt))

## Requirements

- **C++ Compiler**
- **CMake**
- [**uv manager**](https://docs.astral.sh/uv/): python project management tool.


## Usage

**Install python dependencies**

```bash
uv sync
```

**Run python script**

```bash
uv run <path/to/script>.py
# e.g. uv run src/test_cpp_extension.py
```

**Build and run C++ Executable**

```bash
# create build directory
mkdir build && cd build

# configure and build
cmake .. && cmake --build .

# run the executable
./cpp_app           # Linux/macOS
cpp_app.exe         # Windows
```
