.PHONY: create_icon
create_icon: 
	convert icons/icon.svg -define icon:auto-resize="256,128,96,64,48,32,16" icons/icon.ico

