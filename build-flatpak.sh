#!/bin/bash
set -e

mkdir -p flatpak-build
cd flatpak-build

flatpak-builder --user --install --force-clean \
    --repo=repo \
    ../repo \
    ../org.ncravino.giduba.json
