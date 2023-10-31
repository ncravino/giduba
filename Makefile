VERSION := $(shell cat VERSION)

default: build

.PHONY: icon
icon:
	convert icons/icon.svg -define icon:auto-resize="256,128,96,64,48,32,16" icons/icon.ico

.PHONY: prepare
prepare: ./build/
	mkdir -p build && cd build && qmake ../Giduba.pro

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


.PHONY: prepare-deb
prepare-deb: build
	rm -rf ./packaging/deb/ && \
	mkdir -p ./packaging/deb/giduba/usr/local/bin && \
	mkdir -p ./packaging/deb/giduba/DEBIAN && \
	VERSION=${VERSION} envsubst < ./templates/control > ./packaging/deb/giduba/DEBIAN/control && \
	cp ./build/Giduba ./packaging/deb/giduba/usr/local/bin/

.PHONY: prepare-tgz
prepare-tgz: build
	rm -rf ./packaging/tgz/ && \
	mkdir -p ./packaging/tgz/giduba/usr/local/bin && \
	cp ./build/Giduba ./packaging/tgz/giduba/usr/local/bin/

.PHONY: deb
deb: prepare-deb
	dpkg-deb --build ./packaging/deb/giduba && \
	mv ./packaging/deb/giduba.deb ./packaging/deb/giduba-${VERSION}.deb

.PHONY: tgz
tgz: prepare-tgz
	cd ./packaging/tgz/ && tar -czf giduba-${VERSION}.tar.gz ./giduba/*

.PHONY: release
release: clean deb tgz
	mkdir -p ./dist/ && \
	cp ./packaging/tgz/giduba-${VERSION}.tar.gz ./dist/ && \
	cp ./packaging/deb/giduba-${VERSION}.deb ./dist/
