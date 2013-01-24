# odf-gen: Simple API to generate OpenDocument documents.
#
#          Copyright Pablo Jorge 2009 - 2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file ../../LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#

ALL = _odsgenerator.so

ROOT = ../..

include $(ROOT)/Config.mk

CFLAGS += $(PY_INCLUDE) -I $(ROOT)/include
LFLAGS += $(PY_LIBS) -lboost_python

all: $(ALL)

_odsgenerator.o: _odsgenerator.cpp $(ROOT)/include/odsgenerator.h

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ -fpic -fPIC
    
%.so: %.o
	$(CXX) -shared -o $@ -fpic -fPIC $< $(LFLAGS)

clean:
	rm -rf $(ALL) *.o *.pyc

