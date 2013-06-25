CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

# Project parameters
SET(PROJECT_NAME pyudt)
SET(PROJECT_DESCRIPTION "PyUDT")
SET(PROJECT_URL "http://github.com/pyudt/pyudt")

# Version
SET(PYUDT_VERSION_MAJOR 0)
SET(PYUDT_VERSION_MINOR 7)
SET(PYUDT_VERSION_PATCH 0)
SET(PYUDT_VERSION "${PYUDT_VERSION_MAJOR}.${PYUDT_VERSION_MINOR}.${PYUDT_VERSION_PATCH}")

# Disable strong error checking
#SET(CXX_DISABLE_WERROR 1)

# Using C++11
SET(CMAKE_CXX_FLAGS "-std=c++11")

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/python.cmake)

# Search for Boost
SET(BOOST_COMPONENTS python)
SEARCH_FOR_BOOST()

# Search for Log4CXX
ADD_REQUIRED_DEPENDENCY("liblog4cxx >= 0.10.0")

# ----------------------------------------
# Search for Python and set install paths
# ----------------------------------------
SET(Python_ADDITIONAL_VERSIONS 2.7)
FINDPYTHON()
MESSAGE("-- Using Python ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
MESSAGE("-- Python include directories: ${PYTHON_INCLUDE_DIRS}")
MESSAGE("-- Python libraries: ${PYTHON_LIBRARIES}")


# Python site-packages folder
EXECUTE_PROCESS(
  COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

SET(PYTHON_SITE_PACKAGES ${PYTHON_SITELIB})

# It **must** go before PROJECT(PyUDT) in order to work (?!)
SET(PYUDT_INSTALL_PATH ${PYTHON_SITE_PACKAGES}/pyudt CACHE PATH "PyUDT's installation path")

# Set include directories
SET(PYUDT_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/package/udt4_ext/include"
)

INCLUDE_DIRECTORIES(${PYUDT_INCLUDE_DIRS})

# Add subdirectories
INCLUDE(package/udt4_ext/src/SourcesLib.cmake)
INCLUDE(package/udt4_ext/include/SourcesLib.cmake)

# FIXME: make something more robust
SET(UDT_LIBS udt)

# FIXME: use this?
#SETUP_PROJECT()

################################################################################


# The following part is based on PyOpenCV's CMakeLists.
# See: http://code.google.com/p/pyopencv/source/browse/trunk/src/CMakeLists.txt

SET(PYTHON_MODULE_LIST)

# -------------------------------------------
#  Write to configuration script 'config.py'
# -------------------------------------------

# Version
FILE(WRITE config.py "from setuptools import Extension, Library
import os.path as op
from glob import glob

PYUDT_VERSION_MAJOR = \"${PYUDT_VERSION_MAJOR}\"
PYUDT_VERSION_MINOR = \"${PYUDT_VERSION_MINOR}\"
PYUDT_VERSION_PATCH = \"${PYUDT_VERSION_PATCH}\"
PYUDT_VERSION = \"${PYUDT_VERSION}\"
")

# Flags
file(APPEND config.py "
CXX_FLAGS = '${CMAKE_CXX_FLAGS}'.split()
LINK_FLAGS = '${CMAKE_EXE_LINKER_FLAGS}'.split()
")

# Include directories
FILE(APPEND config.py "include_dirs = [")
FOREACH(DIR ${PYUDT_INCLUDE_DIRS})
  STRING(REPLACE "\\" "/" DIR ${DIR})
  FILE(APPEND config.py "\"${DIR}\", ")
ENDFOREACH()
FILE(APPEND config.py "]
")

# Library directories
FILE(APPEND config.py "library_dirs = [")
FOREACH(DIR ${PYUDT_LIBRARY_DIRS})
  FILE(APPEND config.py "\"${DIR}\", ")
ENDFOREACH()
FILE(APPEND config.py "]
")

# Libraries
FILE(APPEND config.py "libraries = [")
FOREACH(DIR ${PYUDT_LIBRARIES})
  FILE(APPEND config.py "\"${DIR}\", ")
ENDFOREACH()
FILE(APPEND config.py "]

def revise_lib(lib):
    path, lib = op.split(op.normpath(lib))
    if not path=='':
        x = lib.rfind('.')
        if x >= 0: lib = lib[:x]
        if lib[:3]=='lib' : lib = lib[3:]
    return lib

libraries = [revise_lib(lib) for lib in libraries]

")

################################################################################

# The following code has been taken and adapted from PyOpenCV
# The code below prints the Python extension for the current system
FILE(WRITE "${CMAKE_BINARY_DIR}/getmodsuffix.py"
"import imp
for s in imp.get_suffixes():
    if s[1] == 'rb' and s[0][0] == '.':
        break
print s[0],
")
# Now execute it and remove any newlines from output
EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_BINARY_DIR}/getmodsuffix.py
    OUTPUT_VARIABLE _modsuffix)
STRING(REPLACE "\n" "" _modsuffix ${_modsuffix}) # Setup specific properties of the Python target


# 'ADD_PYUDT_MODULE' macro
MACRO(ADD_PYUDT_MODULE _NAME)
    FILE(GLOB ${_NAME}_SRC package/${_NAME}_ext/src/*.cpp)
    ADD_LIBRARY(${_NAME}_ext MODULE ${${_NAME}_SRC})
    TARGET_LINK_LIBRARIES(${_NAME}_ext ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${UDT_LIBS})
    PKG_CONFIG_USE_DEPENDENCY(${_NAME}_ext liblog4cxx)
    # rpath
    SET_TARGET_PROPERTIES(${_NAME}_ext PROPERTIES
        INSTALL_RPATH ${PYUDT_INSTALL_PATH}
        BUILD_WITH_INSTALL_RPATH TRUE
        SKIP_BUILD_RPATH FALSE
        PREFIX "" # There is no prefix even on UNIXes
        SUFFIX "${_modsuffix}" # The extension got from Python libraries
    )
    INSTALL(TARGETS ${_NAME}_ext DESTINATION ${PYUDT_INSTALL_PATH})
    #INSTALL(FILES package/pyudt/${_NAME}.py DESTINATION ${PYUDT_INSTALL_PATH})
    SET(PYTHON_MODULE_LIST ${PYTHON_MODULE_LIST} ${_NAME})

    # write to config.py
    FILE(APPEND config.py "${_NAME}_ext = Extension('pyudt.${_NAME}_ext',
    sources=glob(op.join('package', '${_NAME}_ext', 'src', '*.cpp')),
    include_dirs=include_dirs+[op.join('package', '${_NAME}_ext', 'include')],
    library_dirs=library_dirs, libraries=libraries+['udt', 'log4cxx', 'boost_python'],
    extra_compile_args=CXX_FLAGS, extra_link_args=LINK_FLAGS)

")
ENDMACRO(ADD_PYUDT_MODULE)

################################################################################

# ---------------------
# Create Python module
# ---------------------

ADD_PYUDT_MODULE(udt4)

# Add modules to pyudt's config.py
FILE(WRITE package/pyudt/config.py "")
FOREACH(MOD_NAME ${PYTHON_MODULE_LIST})
    FILE(APPEND package/pyudt/config.py "from ${MOD_NAME} import *
")
ENDFOREACH()

# Add modules to the install script's config.py
FILE(APPEND config.py "extension_list = [")
FOREACH(MOD_NAME ${PYTHON_MODULE_LIST})
    FILE(APPEND config.py "${MOD_NAME}_ext, ")
ENDFOREACH()
FILE(APPEND config.py "]
")

################################################################################

# --------------
#  Installation
# --------------

INSTALL(FILES package/pyudt/__init__.py package/pyudt/config.py DESTINATION ${PYUDT_INSTALL_PATH})

################################################################################

# Enable testing for the project
ENABLE_TESTING()

#FILE(COPY test/test_udt4/test_bindings.py DESTINATION .)
ADD_TEST(test_bindings test_bindings.py)

# FIXME: use this?
#SETUP_PROJECT_FINALIZE()
