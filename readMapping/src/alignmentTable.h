
#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "alignmentCell.h"

using namespace std;

#define NEGATIVE_INF -999

class AlignmentTable{
    private:
    string *_s1; //top of the table, what we compare the other string to
    string *_s2;

    int _numRows;
    int _numCols;

    int _score_match;
    int _score_mismatch;
    int _score_h;
    int _score_g;

    string _s1Seq;
    string _s2Seq;
    string _middleString;

    vector<vector <tableCell> > _cellTable;

    void scoreCell(int x, int y, int mode);
    int scoreDel(int x, int y);
    int scoreIns(int x, int y);
    int scoreSub(int x, int y);

    int selectMax(int a, int b, int c);

    void initTable();

    void fillTableGlobal();
    void fillTableLocal();

    void retraceGlobal();
    int retraceLocal();

  public:
    
    void setSequences(string *s1, string *s2);
    void setScores(int match, int mismatch, int h, int g);
    void globalAlign();
    void localAlign();
    void localAlign(string *s1, string *s2, int *matches, int *alignLen);
    vector<string> getAlignments();
    void printTable();
    void printSequences();
};