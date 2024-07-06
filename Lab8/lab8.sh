#!/bin/bash
g++ -c client.cpp
g++ -o client client.o -lpthread
g++ -c server.cpp
g++ -o server server.o -lpthread
