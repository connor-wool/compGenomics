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
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class AffineScorer
{
    private:
        int matchScore;
        int misMatchScore;
        int h;
        int g;

    public:
        int scoreCell(int row, int col);
        int parseFile(string paramFileName);
};

int AffineScorer::parseFile(string paramFileName){
    if(paramFileName.empty()){
        cout << "Error parsing params file, no filename provided.";
        return -1;
    }

    ifstream paramFileStream;
    string line, key;
    int value;
    paramFileStream.open(paramFileName, ifstream::in);
    if(paramFileStream.is_open()){
        while(getline(paramFileStream, line)){
            if(line.empty()) {
                continue;
            }
            stringstream s = stringstream(line);
            s >> key;
            s >> value;
            if(key.compare("match") == 0){
                this->matchScore = value;
            }
            if(key.compare("mismatch") == 0){
                this->misMatchScore = value;
            }
            if(key.compare("h") == 0){
                this->h = value;
            }
            if(key.compare("g") == 0){
                this->g = value;
            }
        }
    }
    cout << "Finished parsing params file\n";
    cout << "match: " << this->matchScore << "\n";
    cout << "mismatch: " << this->misMatchScore << "\n";
    cout << "h: " << this->h << "\n";
    cout << "g: " << this->g << "\n";
}

int AffineScorer::scoreCell(int row, int col){
    return 1;
}

#endif