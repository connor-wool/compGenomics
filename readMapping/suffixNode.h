/*
suffixNode.h
summary: describes a data structure for a single node in the suffix tree.
Written by Connor Wool, April 2018
*/

#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "stringPoint.h"

class SuffixNode
{
public:
  int _id;

  SuffixNode *_suffixLink;
  SuffixNode *_parent;

  std::string *_source;

  SP _edge;
  //int _edgeStartIndex;
  //int _edgeLength;
  int _stringDepth;

  std::vector<SuffixNode *> _children;
  //SuffixNode *_children;

  bool _isInternal;
  bool _isRoot;

  int _start_leaf_index;
  int _end_leaf_index;

  //constructor
  SuffixNode(std::string *source);

  //parent management
  void setParent(SuffixNode *p);
  SuffixNode *getParent();

  //child management
  void addChild(SuffixNode *n);
  void removeChild(SuffixNode *n);
  std::vector<SuffixNode *> getChildren();

  //edge label management
  void setLabel(int start, int length);
  SP getLabel();
  void trimLabelFront(int howMuch);
  void trimLabelBack(int howMuch);

  //suffixLink management
  void
  setSL(SuffixNode *n);
  SuffixNode *getSL();
  bool linkKnown();

  //node ID management
  void ID(int id);
  int ID();

  //manage depth
  void depth(int d);
  int depth();

  //check and set status as root node
  void isRoot(bool value);
  bool isRoot();

  //check and set status as internal node
  void isInternal(bool value);
  bool isInternal();

  //check and set status as leaf node
  void isLeaf(bool value);
  bool isLeaf();

  std::string label();
};