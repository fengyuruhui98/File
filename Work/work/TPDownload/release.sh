#!/bin/sh
exe="shanghai"
des="./release/libc.so"
deplist=$(ldd $exe | awk '{if (match($3,"/")){printf("%s "),$3 } }')
mv $(deplist) ./release/libc.so


