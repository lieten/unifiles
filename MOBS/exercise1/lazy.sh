#!/bin/bash
mkdir lazycode
cd lazycode
touch autocode.c
echo \#include \<stdio.h\> >> autocode.c
echo int main \(void\){ >> autocode.c
echo 'printf("Hello World!\n");' >> autocode.c
echo return 0\; >> autocode.c
echo } >> autocode.c
gcc -o autocode autocode.c
./autocode >> greetings.txt
cat greetings.txt
