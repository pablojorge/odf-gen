ALL = example

CFLAGS += -I ../

all: $(ALL)

test: example
	./example > test.xml
	xmllint --format test.xml
	cd ..; ./ods-gen.sh examples/test.xml examples/test.ods; cd -

%: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(ALL)
