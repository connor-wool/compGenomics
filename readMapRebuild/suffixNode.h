/* suffixNode.h
summary: describes a data structure for a single node in the suffix tree.
Written by Connor Wool, April 2018 */

#include <stdlib.h>
#include <string.h>

#pragma once

typedef struct StringPoint{
    int start;
    int length;
}SP;

typedef struct SuffixNode
{
  int id;
  SP edge;
  struct SuffixNode *parent;
  struct SuffixNode *children[6];
  struct SuffixNode *suffixLink;
  int numChildren;
  int childArrSize;
  int startLeafIndex;
  int endLeafIndex;
  int depth;
  bool isInternal;
  bool isLeaf;
  bool isRoot;
}NODE;

void initNode(NODE* n);
void addChild(char* source, NODE* parent, NODE* child);
void removeChild(NODE* parent, NODE* child);
void trimLabelFront(NODE* n, int howMuch);
void trimLabelBack(NODE* n, int howMuch);