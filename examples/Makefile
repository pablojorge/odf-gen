include ../Config.mk

ALL = example

CFLAGS += -I $(INC_PREFIX)/$(INC_DIR)

all: $(ALL)

test: example
	./example > test.xml
	xmllint --format test.xml
	cd ..; ./ods-gen.sh examples/test.xml examples/test.ods; cd -

example: example.cpp ../include/ods_generator.h

%: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ALL)
