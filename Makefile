
default: build

.PHONY: icon
icon:
	convert icons/icon.svg -define icon:auto-resize="256,128,96,64,48,32,16" icons/icon.ico

.PHONY: prepare
prepare: ./build/
	mkdir -p build && cd build && qmake ../Giduba.pro

.PHONY: build
build: prepare
	cd build && make clean && make

.PHONY: run
run:
	./build/Giduba


