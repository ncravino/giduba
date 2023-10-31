objdump -x ./build/Giduba  | grep NEEDED | sed 's/^[[:space:]]*NEEDED[[:space:]]*//' | xargs dpkg-query -S
