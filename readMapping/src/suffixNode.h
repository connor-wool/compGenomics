/*
suffixNode.h
summary: describes a data structure for a single node in the suffix tree.
Written by Connor Wool, April 2018
*/

#pragma once

#include <vector>
#include <string>
#include <iostream>

typedef struct StringPoint{
    int start;
    int length;
}SP;

class SuffixNode
{
  public:
  SuffixNode* _suffixLink;
  SuffixNode* _parent;
  std::string* _source;
  std::vector<SuffixNode *> _children;
  int _id;
  int _stringDepth;
  int _start_leaf_index;
  int _end_leaf_index;
  SP _edge;
  bool _isInternal;
  bool _isRoot;
  bool _isLeaf;

  //constructor
  SuffixNode(std::string *source);

  //child management
  void addChild(SuffixNode *n);
  void removeChild(SuffixNode *n);

  //edge label management
  void trimLabelFront(int howMuch);
  void trimLabelBack(int howMuch);
};