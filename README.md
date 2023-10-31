# Giduba

![A notepad written in unknown cursive](./icons/icon.png)

A lean, small, and simple text editor / notepad alternative in QT.

The goal of this editor is to be a drop in replacement for MS Notepad in Linux, as a very simple and fast text editor.

This is not a code editor.

Giduba is the sumerian word for writing utensils.

## Features

- Read unicode files
- Write UTF8
- Character, word, line counter
- Word Wrapping
- Fast and Small (~55KB)

## Todo

- Add Search & Replace
- ~~Add configuration file~~
- ~~Fix status bar focus bugs~~
- Make sure icon works in KDE
- ~~Save window state (dimensions et al.)~~
- Add tests
- Create .deb and .tar.gz releases

## Configuration

Configuration is done using the file `$HOME/.config/giduba/giduba.conf`.

### Default Configuration

The file will be created and filled with the default configuration on the first run.

```
[General]
always_show_bar=true
default_font_size=12
default_wrap=true
max_file_size_mb=512
```

### Configuration Keys

- `always_show_bar`:  a boolean that defines whether or not to always show the scroll bars (note that the horizontal will be hidden if wrap is true).
- `default_font_size`: an integer defining the default font size for the editor
- `default_wrap`: a boolean defining wether or not to wrap text (acording to window dimensions)
- `max_file_size_mb`: an integer to limit on the maximum file size that can be opened. In megabytes.

Any missing key will be replaced by the default.

## Screenshots

![A screen shot of giduba in kde plasma](./images/screenshot.png)

