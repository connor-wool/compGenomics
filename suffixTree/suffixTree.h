#pragma once

#include <string>
#include "suffixNode.h"
using namespace std;

//longest repeat struct, used for pulling data out of the tree
struct lrs
{
  string repeatString;
  vector<int> index;
};

class SuffixTree
{
private:
  SuffixNode *root;
  int number_nodes;
  int number_leaf_nodes;
  int number_internal_nodes;
  string sourceString;
  string sourceAlphabet;

  SuffixNode *FindPath(SuffixNode *start, int startIndex, int stringLength, int suffixNumber);
  SuffixNode *NodeHops(SuffixNode *start, int startIndex, int stringLength);
  bool VerifyAlphabet(string input, string alphabet);
  int dfsTraverseHelper(SuffixNode *start, int lineCtl);
  void renumberInternals();
  int renumberInternalsHelper(SuffixNode *start, int value);
  void BWTHelper(SuffixNode *start, string source);
  int sumInternalNodeDepth(SuffixNode *start);
  int findMaxInternalNodeDepth(SuffixNode *start);
  struct lrs longestRepeatHelper(SuffixNode *start);

public:
  SuffixTree();
  string SourceString();
  void Construct(string input, string alphabet);
  void DisplayChildren(SuffixNode *u);
  void dfsTraverse();
  void BWT();
  void PrintTreeStatistics();
  void FindLongestMatchingRepeat();
};