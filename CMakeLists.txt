CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

SET(PROJECT_NAME pyudt)
SET(PROJECT_DESCRIPTION "PyUDT")
SET(PROJECT_URL "http://github.com/pyudt/pyudt")

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/python.cmake)

# Search for Boost
SET(BOOST_COMPONENTS python)
SEARCH_FOR_BOOST()

# Search for Log4CXX
ADD_REQUIRED_DEPENDENCY("liblog4cxx >= 0.10.0")

# Search for Python
FINDPYTHON()

# Add Boost path to include directories
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Add Python path to include directories
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_BINARY_DIR}"
)

# Add subdirectories
INCLUDE(src/SourcesLib.cmake)
INCLUDE(include/SourcesLib.cmake)

# Create Python module
PYTHON_ADD_MODULE(pyudt ${PYUDT_SOURCE} ${PYUDT_HEADERS})
PKG_CONFIG_USE_DEPENDENCY(pyudt liblog4cxx)
TARGET_LINK_LIBRARIES(pyudt udt)

# Enable testing for the project
ENABLE_TESTING()

#FILE(COPY test/test_udt4/test_bindings.py DESTINATION .)
ADD_TEST(test_bindings test_bindings.py)
