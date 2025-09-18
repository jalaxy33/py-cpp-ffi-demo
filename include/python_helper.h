#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace py = pybind11;

// Configure Python home environment (must be called before interpreter initialization)
void configurePythonHome()
{
#ifdef _WIN32
#ifdef PYTHON_HOME_PATH
    SetEnvironmentVariableA("PYTHONHOME", PYTHON_HOME_PATH);
#endif
#endif
}

// Setup virtual environment paths (must be called after interpreter initialization)
void setupVirtualEnvironmentPaths()
{
#ifdef VENV_SITE_PACKAGES_PATH
    std::string venv_site_packages = VENV_SITE_PACKAGES_PATH;
    
    // Convert backslashes to forward slashes for Python path compatibility
    for (char &c : venv_site_packages)
    {
        if (c == '\\')
            c = '/';
    }

    // Add virtual environment path to sys.path if it exists and not already added
    std::string setup_code = 
        "import sys\n"
        "import os\n"
        "venv_path = r'" + venv_site_packages + "'\n"
        "if os.path.exists(venv_path) and venv_path not in sys.path:\n"
        "    sys.path.insert(0, venv_path)\n";

    py::exec(setup_code);
#endif
}
