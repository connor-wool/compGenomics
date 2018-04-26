/*
Connor Wool,
Spring 2018
This class is designed to hold all that data about two
string DNA sequences. 
*/

#pragma once
#define NEGATIVE_INFINITY -999

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "affineScorer.hpp"
#include "fastaParser.hpp"
#include "geneSequence.h"

using namespace std;

//holds information about a cell of the table
struct tableCell
{
    int matchScore;
    int insertionScore;
    int deletionScore;
};

enum Direction
{
    SUBSTITUTION,
    INSERTION,
    DELETION
};

class AlignmentTable
{
  private:
    /*  s1 and s2 store the strings during the sequencing computation.
        The sequenced strings are then pushed into s1_sequenced and s2_sequenced,
        with chars marking where gaps have been added to each. As a check,
        the lengths of s1_sequenced and s2_sequenced should match.
        s3 is the string that will fit in between s1_sequenced and s2_sequenced  */

    string s1, s2;
    int nrows, ncols;
    string s1_sequenced, s2_sequenced, s3_sequenced;

    //used for the local alignment algorithm.
    int local_align_i;
    int local_align_j;
    int local_align_max;

    //Stores cells during computation
    vector<vector<tableCell>> internal_table;

    AffineScorer scorer;            //stores scores from param file
    FastaParser parser;             //parse the input file and store data;
    vector<GeneSequence> sequences; //stores sequences from parser

    //a function to score a specific cell in the table
    void scoreCell(int i, int j, int mode);
    int scoreDel(int i, int j);
    int scoreIns(int i, int j);
    int scoreSub(int i, int j);
    int getScore(int i, int j);
    Direction getDirection(int i, int j);

  public:
    void initTable();

    void fillTable(int mode);

    void initSequences(string sequenceFile)
    {
        this->parser.setInputFile(sequenceFile);
        this->parser.readFile();
        sequences = this->parser.getSequences();
    }

    void initScorer(string paramFileName)
    {
        this->scorer.parseFile(paramFileName);
    }

    void globalAlign();
    void localAlign();
    void printTable();
    void retrace(int mode);
};
