#!/bin/bash
cd Server/Server
gcc -o servC *.c
mv servC ../..
cd ../..
cd Logger/Logger
gcc -o loggerC *.c
mv loggerC ../..
cd ../..
cd Admin/Admin
gcc -o adminC *.c
mv adminC ../..
cd ../..
cd Terminal/Terminal
gcc -o terminalC *.c
mv terminalC ../..
cd ../..