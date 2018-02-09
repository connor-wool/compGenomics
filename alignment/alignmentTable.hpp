/*
Connor Wool,
Spring 2018
This class is designed to hold all that data about two
string DNA sequences. 
*/

#ifndef ALIGNMENTTABLE_H
#define ALIGNMENTTABLE_H

#include <vector>
#include <string>

#include "affineScorer.hpp"
#include "fastaParser.hpp"
#include "geneSequence.hpp"

using namespace std;

enum Direction
{
    //insertions from left, matches on diagonal, deletions from top
    INS, MATCH, DEL
};

//holds information about a cell of the table
typedef struct tableCell{
    int score;
    Direction direction;
};

class AlignmentTable{
    private:
        string s1;
        string s2;
        int nrows;
        int ncols;
        vector<vector<tableCell>> internal_table;
        AffineScorer internal_scorer;

    public:
        void initTable();

        void initStrings(string new_s1, string new_s2){
            this->s1 = new_s1;
            this->s2 = new_s2;
        }

        void initScorer(AffineScorer a){
            this->internal_scorer = a;
        }

        void globalAlign();
        void localAlign();
};

void AlignmentTable::initTable(){
    //leave room for 0-col in the first row and col
    this->nrows = 1 + s1.length();
    this->ncols = 1 + s1.length();

    for(int i = 0; i < nrows; i++){
        vector<tableCell> temp;
        internal_table[i] = temp;
    }
}

void AlignmentTable::globalAlign(){

}

void AlignmentTable::localAlign(){

}

#endif