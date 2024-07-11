PYTHON = python3

build:
	$(PYTHON) setup.py build_ext -i

check: build
	$(PYTHON) -m unittest discover -v tests

clean:
	$(PYTHON) setup.py clean

dist: build
	$(PYTHON) setup.py sdist --force-manifest

.PHONY: build check clean dist
