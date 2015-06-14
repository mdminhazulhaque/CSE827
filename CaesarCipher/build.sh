#!/bin/bash

if [ "$1" == "clean" ]
then
	rm -f CaesarCipher
else
	g++ -o CaesarCipher CaesarCipher.cpp
fi

