#!/bin/bash

if [ "$1" == "clean" ]
then
	rm -f MonoalphabeticCipher
else
	g++ -o MonoalphabeticCipher MonoalphabeticCipher
fi

