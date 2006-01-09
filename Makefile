PYTHON=python2.4

build:
	$(PYTHON) setup.py build_ext -i

.PHONY: build
