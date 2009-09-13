include Config.mk

TARGET = odfgen

all: $(TARGET)

$(TARGET):
	$(MAKE) -C $@

install-headers:
	mkdir -p $(INC_PREFIX)/$(INC_DIR)
	cp -av include/*.h $(INC_PREFIX)/$(INC_DIR)

uninstall-headers:
	rm -rf $(INC_PREFIX)/$(INC_DIR)

install-python-modules: odfgen
	cp -av odfgen/*.py odfgen/*.so $(PY_LIB_PREFIX)

uninstall-python-modules:
	rm -rf $(PY_LIB_PREFIX)/odfgen

install-bin:
	mkdir -p $(LIB_PREFIX)/$(LIB_DIR)
	cp -av src/*.py src/*.xslt $(LIB_PREFIX)/$(LIB_DIR)
	cp odsgen $(BIN_PREFIX)

uninstall-bin:
	rm -rf $(LIB_PREFIX)/$(LIB_DIR)
	rm -rf $(BIN_PREFIX)/odsgen

install: install-headers install-python-modules install-bin
uninstall: uninstall-headers uninstall-python-modules uninstall-bin

.PHONY: $(TARGET)
