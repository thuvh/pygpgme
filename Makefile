PYTHON = python2.4

build:
	$(PYTHON) setup.py build_ext -i

check: build
	$(PYTHON) test_all.py -v

.PHONY: build check
