/*
Connor Wool, Spring 2018
A library file used in computing sequence alignments.
The AffineScorer object contains data about how to score
an affine alignment method. This scorer has been designed
as a separate object from the table of cells, so that
it can be used for either local or global alignment
cases.
*/

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class AffineScorer
{
    private:
        int matchScore;
        int misMatchScore;
        int hScore;
        int gScore;

    public:
        int scoreCell(int row, int col);
        int parseFile(string paramFileName);
        int match(){
            return this->matchScore;
        }
        int misMatch(){
            return this->misMatchScore;
        }
        int h(){
            return this->hScore;
        }
        int g(){
            return this->gScore;
        }
};