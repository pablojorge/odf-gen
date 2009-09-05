ALL = ods_generator_.so

CFLAGS += -I /usr/include/python2.6/ -I ..
LFLAGS += -lboost_python

all: $(ALL)

ods_generator_.o: ods_generator_.cpp ../include/ods_generator.h

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
    
%.so: %.o
	$(CXX) $(LFLAGS) -shared -o $@ -fpic -fPIC $< 

clean:
	rm -rf $(ALL) *.o

