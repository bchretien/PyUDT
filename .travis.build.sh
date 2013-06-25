#!/bin/sh
set -e

# Directories.
root_dir=`pwd`
install_dir="$root_dir/_travis/install"

# Shortcuts.
git_clone="git clone --quiet --recursive"

# Create layout.
rm -rf "$install_dir"
mkdir -p "$install_dir"
mkdir -p "$install_dir/lib"
mkdir -p "$install_dir/include/udt"

# Current Python version
export PYTHON_VERSION=`python -c 'import sys; print(sys.version[:3])'`

# Setup environment variables.
export LD_LIBRARY_PATH="$install_dir/lib:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$install_dir/lib/pkgconfig:$PKG_CONFIG_PATH"
export CFLAGS="-I$install_dir/include -L$install_dir/lib"
export CXXFLAGS="-I$install_dir/include -L$install_dir/lib"
export PYTHONPATH="/home/travis/virtualenv/python$PYTHON_VERSION/lib/python$PYTHON_VERSION/site-packages:$PYTHONPATH"

pythonsite_dir=`python -c "import sys, os; print os.sep.join(['lib', 'python' + sys.version[:3], 'site-packages'])"`
export PYTHONPATH="$install_dir/$pythonsite_dir:$PYTHON_PATH"

echo "g++ version: "
g++ --version

# Build package
echo "Building UDT..."

# move library and headers to appropriate location
cd ${root_dir}/udt/udt4

echo "Installing package files..."
make -j1
cp src/libudt.so ${install_dir}/lib/
cp src/udt.h ${install_dir}/include/udt/udt.h

cd ../..

# Python-based install
#echo "Calling setup.py..."
#python ${root_dir}/setup.py build
#echo "Installing PyUDT..." \
#python ${root_dir}/setup.py install -v --prefix="${install_dir}"

# CMake-based install
echo "Building Python bindings..."
mkdir build && cd build
cmake .. -DPythonInterp_FIND_VERSION=${PYTHON_VERSION} \
         -DPythonLibs_FIND_VERSION=${PYTHON_VERSION} \
         -DCMAKE_INSTALL_PREFIX=${install_dir} \
         -DCMAKE_CXX_FLAGS="-I${install_dir}/include" \
         -DCMAKE_MODULE_LINKER_FLAGS="-L${install_dir}/lib"
make VERBOSE=1
make install
cd ..

echo "Running tests..."
./test/run_tests.py -v
