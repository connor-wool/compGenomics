#pragma once

#include <string>
#include "suffixNode.h"
using namespace std;

class SuffixTree
{
  private:
    SuffixNode *root;
    int number_nodes;
    int number_leaf_nodes;
    int number_internal_nodes;
    string sourceString;
    string sourceAlphabet;

    SuffixNode* FindPath(SuffixNode *start, string s, int suffixNumber);
    SuffixNode* NodeHops(SuffixNode *start, string beta);
    bool VerifyAlphabet(string input, string alphabet);
    int dfsTraverseHelper(SuffixNode *start, int lineCtl);
    void renumberInternals();
    int renumberInternalsHelper(SuffixNode *start, int value);
    void BWTHelper(SuffixNode *start, string source);
    int sumInternalNodeDepth(SuffixNode *start);
    int findMaxInternalNodeDepth(SuffixNode *start);

  public:
    SuffixTree();
    void Construct(string input, string alphabet);
    void DisplayChildren(SuffixNode *u);
    void dfsTraverse();
    void BWT();
    void PrintTreeStatistics();
};