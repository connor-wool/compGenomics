/*
Connor Wool, Spring 2018
A library file used in computing sequence alignments.
The AffineScorer object contains data about how to score
an affine alignment method. This scorer has been designed
as a separate object from the table of cells, so that
it can be used for either local or global alignment
cases.
*/

#ifndef AFFINESCORER_H
#define AFFINESCORER_H

#include <vector>
#include <string>

using namespace std;

class AffineScorer
{
    private:
        int matchScore;
        int misMatchScore;
        int h;
        int g;

    public:
        void setH(int newh){
            this->h = newh;
        }
        void setG(int newg){
            this->g = newg;
        }
        void setMatchScore(int newMatchScore){
            this->matchScore = newMatchScore;
        }
        void setMismatchScore(int newMismatchScore){
            this->misMatchScore = newMismatchScore;
        }
        int scoreCell(int row, int col);
};

int AffineScorer::scoreCell(int row, int col){
    
}

#endif