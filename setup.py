#
#   @author         Christopher J. Hanks    <develop@cjhanks.name>
#   @license        py-udt4 is GPLv3, however it must be linked against UDT4
#                   libraries to be of use.  UDT4 license is below.
#   @date           12/11/2012
#
#   # - UDT LICENSE ------------------------------------------------------
#   Copyright (c) 2001 - 2011, The Board of Trustees of the University of
#   Illinois.  All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
#
#   * Redistributions of source code must retain the above
#     copyright notice, this list of conditions and the
#     following disclaimer.
#
#   * Redistributions in binary form must reproduce the
#     above copyright notice, this list of conditions
#     and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#
#   * Neither the name of the University of Illinois
#     nor the names of its contributors may be used to
#     endorse or promote products derived from this
#     software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#   -----------------------------------------------------------------------
#

from setuptools import setup, find_packages, Extension, Library

from glob import glob
import sys
import os
import os.path as op
import distutils.spawn as ds
import distutils.dir_util as dd

try:
    import config as C
except ImportError: # no config.py file found
    if ds.find_executable('cmake') is None:
        print "Error: unable to configure PyUDT!"
        print
        print "Starting from version 0.7.0, PyUDT relies on the CMake build"
        print "tool (http://www.cmake.org/) to configure. However, CMake is not"
        print "found in your system. Please install CMake before running the"
        print "setup file."
        print
        print "Once CMake is installed, you can also manually configure PyUDT"
        print "by running the following commands:"
        print "    mkdir build"
        print "    cd build"
        print "    cmake .. -DPythonInterp_FIND_VERSION=2.7 -DPythonLibs_FIND_VERSION=2.7"
        print "    cd .."
        sys.exit(-1)

    print "Configuring PyUDT via CMake..."
    cur_dir = os.getcwd()
    new_dir = op.join(op.split(__file__)[0], 'build')
    dd.mkpath(new_dir)
    os.chdir(new_dir)
    try:
        ds.spawn(['cmake', '..', '-DPythonInterp_FIND_VERSION=2.7', '-DPythonLibs_FIND_VERSION=2.7'])
    except ds.DistutilsExecError:
        print "Error: error occurred while running CMake to configure PyUDT."
        print "You may want to manually configure PyUDT by running CMake's"
        print "tools:"
        print "    mkdir build"
        print "    cd build"
        print "    cmake .. -DPythonInterp_FIND_VERSION=2.7 -DPythonLibs_FIND_VERSION=2.7"
        print "    cd .."
        sys.exit(-1)
    os.chdir(cur_dir)
    import config as C

setup(
        name            = 'pyudt',
        version         = C.PYUDT_VERSION,
        description     = 'Python bindings for UDT',
        author          = 'Christopher J. Hanks',
        author_email    = 'develop@cjhanks.name',
        url             = 'http://cjhanks.name/projects/py-udt.php',
        packages        = find_packages('package'),
        package_dir     = { '' : 'package' },
        ext_modules     = C.extension_list,
        license         = 'GPLv3',
        download_url    = 'https://github.com/cjhanks/PyUDT',
        keywords        = ['udt', 'pyudt', 'udt4'],
        classifiers     = [
            'Development Status :: 3 - Alpha',
            'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
            'Topic :: Software Development :: Libraries',
            'Programming Language :: Python :: 2.6',
            'Programming Language :: Python :: 2.7'
            ]
    )
