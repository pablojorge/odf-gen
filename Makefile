include Config.mk

ALL = examples odfgen

all: $(ALL)

$(ALL):
	$(MAKE) -C $@
	echo $(PY_LIB_PREFIX)

install:
	mkdir -p $(INC_PREFIX)/$(INC_DIR)
	cp -av include/* $(INC_PREFIX)/$(INC_DIR)
	cp -rav odfgen $(PY_LIB_PREFIX)
	mkdir -p $(LIB_PREFIX)/$(LIB_DIR)
	cp -av src/* $(LIB_PREFIX)/$(LIB_DIR)
	cp odsgen $(BIN_PREFIX)

uninstall:
	rm -rf $(INC_PREFIX)/$(INC_DIR)
	rm -rf $(PY_LIB_PREFIX)/odfgen
	rm -rf $(LIB_PREFIX)/$(LIB_DIR)
	rm -rf $(BIN_PREFIX)/odsgen

.PHONY: $(ALL)
