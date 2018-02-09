/*
Connor Wool,
Spring 2018
This class is designed to hold all that data about two
string DNA sequences. 
*/

#pragma once

#include <vector>
#include <string>

#include "affineScorer.hpp"
#include "fastaParser.hpp"
#include "geneSequence.hpp"

using namespace std;

//holds information about a cell of the table
struct tableCell{
    int matchScore;
    int insertionScore;
    int deletionScore;
};

class AlignmentTable{
    private:
        string s1;
        string s2;
        int nrows;
        int ncols;
        vector<vector<tableCell>> internal_table;
        AffineScorer score;     //stores scores from param file
        FastaParser parser;     //parse the input file and store data;
        vector<GeneSequence> sequences; //stores sequences from parser

    public:
        void initTable();

        void initSequences(string sequenceFile){
            this->parser.setInputFile(sequenceFile);
            this->parser.readFile();
            sequences = this->parser.getSequences();
        }

        void initStrings(string new_s1, string new_s2){
            this->s1 = new_s1;
            this->s2 = new_s2;
        }

        void initScorer(string paramFileName){
            this->score.parseFile(paramFileName);
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