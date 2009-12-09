ALL = _odsgenerator.so

CFLAGS += -I /usr/include/python2.6/ -I ..
LFLAGS += -lboost_python-mt

all: $(ALL)

_odsgenerator.o: _odsgenerator.cpp ../odfgen/odsgenerator.h

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
    
%.so: %.o
	$(CXX) $(LFLAGS) -shared -o $@ -fpic -fPIC $< 

clean:
	rm -rf $(ALL) *.o *.pyc

