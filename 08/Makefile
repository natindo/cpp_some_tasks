PYTHON ?= python3

all: install

build:
	$(PYTHON) setup.py build

install: build
	$(PYTHON) setup.py install --user  # или просто install, если нужно в system-wide

clean:
	$(PYTHON) setup.py clean --all
	rm -rf build *.egg-info
