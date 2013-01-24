# odf-gen: Simple API to generate OpenDocument documents.
#
#          Copyright Pablo Jorge 2009 - 2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#

INC_PREFIX=/usr/local/include
INC_DIR=odfgen

PY_GET_LIB_PREFIX += from distutils.sysconfig import get_python_lib;
PY_GET_LIB_PREFIX += print get_python_lib()

PY_LIB_PREFIX=$(shell python -c "$(PY_GET_LIB_PREFIX)")
PY_LIB_DIR=odfgen

PY_INCLUDE=$(shell python-config --includes)
PY_LIBS=$(shell python-config --libs)

LIB_PREFIX=/usr/local/lib
LIB_DIR=odfgen

BIN_PREFIX=/usr/local/bin

