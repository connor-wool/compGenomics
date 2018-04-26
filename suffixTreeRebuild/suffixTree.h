/*
suffixTree.h
summary: describes a data structure for a tree of SuffixNodes
Written by Connor Wool, April 2018
*/

#pragma once

#include <string>

#include "suffixNode.h"

using namespace std;

struct lrs{
    string repeatString;
    vector<int> index;
};

class SuffixTree{

    private:
    SuffixNode *_root;
    int _numNodes;
    int _numLeaves;
    int _numInternal;
    string _source;
    string _alphabet;

    SuffixNode *findPath(SuffixNode *n, SP section, int suffixNumber);
    SuffixNode *nodeHops(SuffixNode *n, SP section);
    bool verifyAlphabet(string input, string alphabet);
    void renumberInternals();
    int renumberInternalsHelper(SuffixNode *n, int value);

    void bwtHelper(SuffixNode *n, string *source);
    //int sumInternalNodeDepth(SuffixNode *n);
    //int findMaxInternalNodeDepth(SuffixNode *n);
    int dfsTraverseHelper(SuffixNode *n);
    //struct lrs longestRepeatHelper(SuffixNode *n);

    public:
    SuffixTree();
    void Construct(string input, string alphabet);
    void PrintChildren(SuffixNode *n);
    void dfsTraverse();
    void BWT();
    void PrintTreeStatistics();
    void FindLongestMatchingRepeat();
};