/* suffixTree.h
summary: describes a data structure for a tree of SuffixNodes
Written by Connor Wool, April 2018 */

#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "suffixNode.h"

using namespace std;

class SuffixTree{

    private:

    //vector<NODE> nodeStorage;
    NODE* root;

    int _numNodes;
    int _numLeaves;
    int _numInternal;

    string _source;
    char *cstring;
    string _alphabet;

    int _x;
    vector<int> _A;
    int _nextIndex;

    SuffixNode *_fl_deepest;
    int _fl_maxDepth;
    int _fl_read_ptr;

    //MARK: Private Functions
    NODE *findPath(NODE *n, SP section, int suffixNumber);
    
    NODE *nodeHops(NODE *n, SP section);
    
    bool verifyAlphabet(string input, string alphabet);
    
    void renumberInternals();
    
    int renumberInternalsHelper(NODE *n, int value);

    void bwtHelper(SuffixNode *n, string *source);
    //int sumInternalNodeDepth(SuffixNode *n);
    //int findMaxInternalNodeDepth(SuffixNode *n);
    int dfsTraverseHelper(SuffixNode *n);
    //struct lrs longestRepeatHelper(SuffixNode *n);

    public:
    SuffixTree();
    void Construct(string input, string alphabet);
    void PrintChildren(NODE *n);
    void dfsTraverse();
    void BWT();
    void PrintTreeStatistics();
    void FindLongestMatchingRepeat();
    void PrepareST();
    void PrepareSTRecursive(NODE *n);
    //void FindLoc(string read, SuffixNode *n);
    vector<int> FindLoc(string read);
};