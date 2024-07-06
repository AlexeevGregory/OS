#!/bin/bash
g++ -c prog2.cpp
g++ -o prog2 prog2.o -lpthread
g++ -c prog1.cpp
g++ -o prog1 prog1.o -lpthread
