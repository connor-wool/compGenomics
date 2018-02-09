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
struct tableCell
{
    int matchScore;
    int insertionScore;
    int deletionScore;
};

class AlignmentTable
{
  private:
    /*  s1 and s2 store the strings during the sequencing computation.
        The sequenced strings are then pushed into s1_sequenced and s2_sequenced,
        with chars marking where gaps have been added to each. As a check,
        the lengths of s1_sequenced and s2_sequenced should match.  */

    string s1, s2;
    int nrows, ncols;
    string s1_sequenced, s2_sequenced;

    //Stores cells during computation
    vector<vector<tableCell>> internal_table;

    AffineScorer scorer;            //stores scores from param file
    FastaParser parser;             //parse the input file and store data;
    vector<GeneSequence> sequences; //stores sequences from parser

    //a function to score a specific cell in the table
    int scoreCell(int i, int j);
    int scoreDel(int i, int j);
    int scoreIns(int i, int j);
    int scoreSub(int i, int j);
    int getScore(int i, int j);

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
};

int AlignmentTable::scoreDel(int i, int j)
{
    if (i > 0)
    {
        //initialize max value to be a small int value
        int max = INT32_MIN / 2;

        //get a pointer to the previous cell we want to reference
        tableCell *previous = &internal_table[i - 1][j];

        if (previous->deletionScore + this->scorer.g() > max)
        {
            max = previous->deletionScore + this->scorer.g();
        }
        if (previous->matchScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->matchScore + this->scorer.h() + this->scorer.g();
        }
        if (previous->insertionScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->insertionScore + this->scorer.h() + this->scorer.g();
        }

        //return the largest score we saw
        return max;
    }
    else
    {
        //if we're on the edge, return a small int value
        return INT32_MIN / 2;
    }
}

int AlignmentTable::scoreIns(int i, int j)
{
    if (j > 0)
    {
        //initialize max value to be a small int value
        int max = INT32_MIN / 2;

        //get a pointer to the previous cell we want to reference
        tableCell *previous = &internal_table[i][j - 1];

        if (previous->deletionScore + this->scorer.g() > max)
        {
            max = previous->deletionScore + this->scorer.h() + this->scorer.g();
        }
        if (previous->matchScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->matchScore + this->scorer.h() + this->scorer.g();
        }
        if (previous->insertionScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->insertionScore + this->scorer.g();
        }

        //return the largest score we saw
        return max;
    }
    else
    {
        //if we're on the edge, return a small int value
        return INT32_MIN / 2;
    }
}

int AlignmentTable::scoreSub(int i, int j)
{

    if (i > 0 && j > 0)
    {
        //initialize max value to be a small int value
        int max = INT32_MIN / 2;

        //get a pointer to the previous cell we want to reference
        tableCell *previous = &internal_table[i - 1][j - 1];

        if (previous->deletionScore > max)
        {
            max = previous->deletionScore;
        }
        if (previous->matchScore > max)
        {
            max = previous->matchScore;
        }
        if (previous->insertionScore > max)
        {
            max = previous->insertionScore;
        }

        //return the largest score we saw, with points for the match or mismatch
        if (s1[i] == s2[j])
        {
            return max + this->scorer.match();
        }
        else
        {
            return max + this->scorer.misMatch();
        }
    }

    else
    {
        //if we're on the edge, return a small int value
        return INT32_MIN / 2;
    }
}

int AlignmentTable::scoreCell(int i, int j)
{
    tableCell *current = &internal_table[i][j];
    current->deletionScore = scoreDel(i, j);
    current->insertionScore = scoreIns(i, j);
    current->matchScore = scoreSub(i, j);
}

int AlignmentTable::getScore(int i, int j){
    tableCell *current = &internal_table[i][j];
    int max = INT32_MIN / 2;
    if (current->matchScore > max){
        max = current->matchScore;
    }
    if (current->deletionScore > max){
        max = current->deletionScore;
    }
    if (current->insertionScore > max){
        max = current->insertionScore;
    }
    return max;
}

//TODO:
//create empty table (vector of vectors)
//set string 1 and string 2 from vector of sequences
//run algorithm on first row and first column to prepare
void AlignmentTable::initTable()
{

    if (this->sequences.size() < 2)
    {
        cout << "not enough sequences... something went wrong... (#=" << this->sequences.size() << "\n";
        exit(1);
    }

    //set the sequences from the parser
    this->s1 = this->sequences[0].sequence;
    this->s2 = this->sequences[1].sequence;

    //set the size of the table from the sequence strings
    //leaves room for indel row/col in the first row and col
    this->ncols = 1 + s1.length();
    this->nrows = 1 + s2.length();

    for (int i = 0; i < nrows; i++)
    {
        vector<tableCell> temp;
        for (int j = 0; j < ncols; j++)
        {
            tableCell t;
            t.deletionScore = 0;
            t.insertionScore = 0;
            t.matchScore = 0;
            temp.push_back(t);
        }
        this->internal_table.push_back(temp);
    }

    cout << "in theory: rows:" << this->nrows << " cols:" << this->ncols << "\n";
    cout << "in practice: rows:" << this->internal_table.size();
    cout << "cols: " << this->internal_table[0].size() << "\n";
}

void AlignmentTable::fillTable(int mode)
{
    tableCell *current;

    if (mode == 0)   //prepare for global alignment
    {
        current = &this->internal_table[0][0];
        current->deletionScore = INT32_MIN / 2;
        current->insertionScore = INT32_MIN / 2;
        current->matchScore = 0;
        for (int i = 1; i < this->internal_table.size(); i++){
            current = &this->internal_table[i][0];
            current->matchScore = INT32_MIN / 2;
            current->insertionScore = INT32_MIN / 2;
            current->deletionScore = this->scorer.h() + (i * this->scorer.g());
        }
        for (int j = 1; j < this->internal_table[0].size(); j++){
            current = &this->internal_table[0][j];
            current->matchScore = INT32_MIN /2;
            current->deletionScore = INT32_MIN / 2;
            current->insertionScore = this->scorer.h() + (j * this->scorer.g());
        }
    }
}

//global initializes the edges using the scoring method
void AlignmentTable::globalAlign()
{
}

//local initialzes both local and global edges to 0
void AlignmentTable::localAlign()
{
}