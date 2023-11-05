VERSION := $(shell cat VERSION)
DATE := $(shell date +"%Y-%m-%d")

default: build

.PHONY: icon
icon:
	convert ./resources/icons/giduba.svg -define icon:auto-resize="256,128,96,64,48,32,16" ./resources/icons/giduba.ico
	convert ./resources/icons/giduba.svg  -resize 256 ./resources/icons/giduba.png

./build/:
	mkdir -p build && cd build && qmake ../Giduba.pro

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


.PHONY: prepare-deb
prepare-deb: build
	rm -rf ./packaging/deb/ && \
	mkdir -p ./packaging/deb/giduba/usr/bin && \
	mkdir -p ./packaging/deb/giduba/usr/share/applications && \
	mkdir -p ./packaging/deb/giduba/usr/share/metainfo && \
	mkdir -p ./packaging/deb/giduba/usr/share/icons/hicolor/scalable/apps/ && \
	mkdir -p ./packaging/deb/giduba/DEBIAN && \
	VERSION=${VERSION} envsubst < ./scripts/packaging//control > ./packaging/deb/giduba/DEBIAN/control && \
	VERSION=${VERSION} DATE=${DATE} envsubst < ./scripts/packaging/com.ncravino.giduba.appdata.xml > ./packaging/deb/giduba/usr/share/metainfo/com.ncravino.giduba.appdata.xml && \
	cp ./scripts/packaging/com.ncravino.giduba.desktop ./packaging/deb/giduba/usr/share/applications/ && \
	cp ./resources/icons/giduba.svg ./packaging/deb/giduba/usr/share/icons/hicolor/scalable/apps/ && \
	cp ./build/Giduba ./packaging/deb/giduba/usr/bin/

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

.PHONY: prepare-appimage
prepare-appimage: build
	rm -rf ./packaging/appimage && \
	mkdir -p ./packaging/appimage/AppDir/usr/bin/ && \
	mkdir -p ./packaging/appimage/AppDir/usr/share/metainfo/ && \
	VERSION=${VERSION} DATE=${DATE} envsubst < ./scripts/packaging/com.ncravino.giduba.appdata.xml > ./packaging/appimage/AppDir/usr/share/metainfo/com.ncravino.giduba.appdata.xml && \
	cp ./scripts/packaging/com.ncravino.giduba.desktop ./packaging/appimage/ && \
	cp ./resources/icons/giduba.svg ./packaging/appimage && \
	cp ./build/Giduba ./packaging/appimage/AppDir/usr/bin/

.PHONY: appimage
appimage: prepare-appimage
	cd ./packaging/appimage/ && \
	linuxdeploy-x86_64.AppImage --appdir AppDir -d ./com.ncravino.giduba.desktop -i ./giduba.svg --output appimage



.PHONY: release
release: clean deb tgz appimage
	mkdir -p ./dist/ && \
	cp ./packaging/tgz/giduba-${VERSION}.tar.gz ./dist/ && \
	cp ./packaging/deb/giduba-${VERSION}.deb ./dist/ && \
	cp ./packaging/appimage/Giduba-x86_64.AppImage ./dist/

.PHONY: deb-install-build-depends
deb-install-build-depends:
	sudo apt update && sudo apt install make qt5-qmake qtbase5-dev build-essential

