/*
Connor Wool - Spring 2018
CPTS 471; Prof. Ananth;

This class defines an object that stores a Gene sequence with it's name.
This is a trivial class, designed for portability and later extension.
*/

#pragma once

#include <string>

using namespace std;

class GeneSequence{
    public:
        string name;
        string sequence;
};