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

