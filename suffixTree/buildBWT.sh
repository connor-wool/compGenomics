#! /bin/bash

#command to build the project from source
g++ -g -std=c++11 -o bwt.out suffixNode.cpp suffixTree.cpp bwtMain.cpp