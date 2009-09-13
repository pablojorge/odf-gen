ALL = odsgenerator_.so

CFLAGS += -I /usr/include/python2.6/ -I ..
LFLAGS += -lboost_python

all: $(ALL)

odsgenerator_.o: odsgenerator_.cpp ../include/odsgenerator.h

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
    
%.so: %.o
	$(CXX) $(LFLAGS) -shared -o $@ -fpic -fPIC $< 

clean:
	rm -rf $(ALL) *.o *.pyc

