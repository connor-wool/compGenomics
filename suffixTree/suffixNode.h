/* suffixNode.cpp - describes a data structure for a node in the suffix tree.
Written by Connor Wool, Spring 2018 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define SN_IS_ROOT 0x01
#define SN_IS_INTERNAL 0x02
#define SN_IS_LEAF 0x04
#define SN_LINK_KNOWN 0x08

using namespace std;

class SuffixNode
{
private:
  int id;

  SuffixNode *suffixLink;
  SuffixNode *parent;
  string *source;

  int el_startIndex;
  int el_length;

  vector<SuffixNode *> children;

  int stringDepth;
  int flags;

public:
  //MARK: Constructors
  SuffixNode(string *source);
  SuffixNode(string *source, int id);
  SuffixNode(string *source, int id, SuffixNode *parent);

  //MARK: manage parents and children
  void setParent(SuffixNode *p);
  SuffixNode *getParent();
  void addChild(SuffixNode *n);
  void removeChild(SuffixNode *n);
  vector<SuffixNode *> getChildren();

  void printChildren();
  void printNode();
  void printDepth();

  //MARK: Manage Edge Label and index
  void setEdgeLabel(int start, int length);
  string getEdgeLabel();
  int getEdgeStart();
  int getEdgeLength();

  void setSuffixLink(SuffixNode *n);
  SuffixNode *getSuffixLink();

  void setId(int id);
  int getId();

  void setStringDepth(int depth);
  int getStringDepth();

  //track status as root node
  void setAsRoot();
  bool isRoot();

  //track status as internal node
  void setAsInternal();
  bool isInternal();

  //track status as leaf node
  void setAsLeaf();
  bool isLeaf();

  void setLinkKnown();
  void setLinkUnknown();
  bool linkKnown();
};