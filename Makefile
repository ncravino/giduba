VERSION := $(shell cat VERSION)
DATE := $(shell date +"%Y-%m-%d")

default: build

.PHONY: icon
icon:
	convert ./resources/icons/giduba.svg -define icon:auto-resize="256,128,96,64,48,32,16" ./resources/icons/giduba.ico
	convert ./resources/icons/giduba.svg  -resize 256 ./resources/icons/giduba.png

./build/:
	mkdir -p build && cd build && qmake6 ../Giduba.pro

.PHONY: prepare
prepare: ./build/

.PHONY: clean
clean: prepare
	rm -rf ./packaging/ && \
	cd build && make clean

.PHONY: build
build: prepare
	cd build && make

.PHONY: run
run:
	./build/Giduba

.PHONY: deb-install-build-depends
deb-install-build-depends:
	sudo apt update && sudo apt install make qmake6 qt6-base-dev build-essential gettext file podman
