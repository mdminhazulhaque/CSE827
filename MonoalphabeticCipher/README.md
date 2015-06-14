# MonoalphabeticCipher

Encrypts and decrypts strings using monoalphabetic substitution formula.

The function `decrypt(string message)` runs brute force and calculates word weight from loaded common word database.

# Features

* Ctrl+C (SIGTERM) handler
* Uptime calculator

# Available Static Functions

`static string encrypt(string message, int key)`

`static string decrypt(string message, int key)`

`key` must be less than 26, and nonzero.

# How To

To build, run `build.sh`

To clean, run `build.sh clean`

# 10000-common-words.txt

This file is used to load word data into memory.
