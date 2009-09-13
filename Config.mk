INC_PREFIX=/usr/local/include
INC_DIR=odf_gen

LIB_PREFIX=/usr/local/lib
LIB_DIR=odf_gen

BIN_PREFIX=/usr/local/bin

PY_GET_LIB_PREFIX += from distutils.sysconfig import get_python_lib;
PY_GET_LIB_PREFIX += print get_python_lib()

PY_LIB_PREFIX=$(shell python -c "$(PY_GET_LIB_PREFIX)")
