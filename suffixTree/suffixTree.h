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
    SuffixNode* FindPath(SuffixNode *start, string s, int suffixNumber);
    SuffixNode* NodeHops(SuffixNode *start, string beta);
    bool VerifyAlphabet(string input, string alphabet);
    void dfsTraverseHelper(SuffixNode *start);
    void renumberInternals();
    int renumberInternalsHelper(SuffixNode *start, int value);

  public:
    SuffixTree();
    void Construct(string input, string alphabet);
    void DisplayChildren(SuffixNode *u);
    void dfsTraverse();
};