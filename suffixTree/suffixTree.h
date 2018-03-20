#pragma once

#include <string>
#include "suffixNode.h"
using namespace std;

class SuffixTree
{
  private:
    SuffixNode *root;
    void FindPath(SuffixNode *start, string s, int suffixNumber);
    void NodeHops(SuffixNode *start, string s);
    bool VerifyAlphabet(string input, string alphabet);

  public:
    SuffixTree();
    void Construct(string input, string alphabet);
    void DisplayChildren(SuffixNode *u);
};