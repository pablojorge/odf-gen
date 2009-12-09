ALL = _odsgenerator.so

ROOT = ../..

CFLAGS += -I /usr/include/python2.6/ -I $(ROOT)/include
LFLAGS += -lboost_python-mt

all: $(ALL)

_odsgenerator.o: _odsgenerator.cpp $(ROOT)/include/odsgenerator.h

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
    
%.so: %.o
	$(CXX) $(LFLAGS) -shared -o $@ -fpic -fPIC $< 

clean:
	rm -rf $(ALL) *.o *.pyc

