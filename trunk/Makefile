include Config.mk

PY_MODULES = python-modules
TARGET = $(PY_MODULES)

all: $(TARGET)

clean:
	$(MAKE) $@ -C $(PY_MODULES)
	$(MAKE) $@ -C examples

$(TARGET):
	$(MAKE) -C $@

install-headers:
	mkdir -p $(INC_PREFIX)/$(INC_DIR)
	cp -av include/*.h $(INC_PREFIX)/$(INC_DIR)

uninstall-headers:
	rm -rf $(INC_PREFIX)/$(INC_DIR)

install-python-modules: $(PY_MODULES)
	mkdir -p $(PY_LIB_PREFIX)/$(PY_LIB_DIR)
	cp -av $(PY_MODULES)/*.py $(PY_MODULES)/*.so $(PY_LIB_PREFIX)/$(PY_LIB_DIR)

uninstall-python-modules:
	rm -rf $(PY_LIB_PREFIX)/$(PY_LIB_DIR)

install-bin:
	mkdir -p $(LIB_PREFIX)/$(LIB_DIR)
	cp -av src/*.py src/*.xslt $(LIB_PREFIX)/$(LIB_DIR)
	cp src/odsgen $(BIN_PREFIX)

uninstall-bin:
	rm -rf $(LIB_PREFIX)/$(LIB_DIR)
	rm -rf $(BIN_PREFIX)/odsgen

install: install-headers install-python-modules install-bin
uninstall: uninstall-headers uninstall-python-modules uninstall-bin

install-cpp: install-headers install-bin
install-python: install-python-modules install-bin

.PHONY: $(TARGET)
