/* suffixNode.cpp - describes a data structure for a node in the suffix tree.
Written by Connor Wool, Spring 2018 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#define SN_IS_ROOT      0x01
#define SN_IS_INTERNAL  0x02
#define SN_IS_LEAF      0x04
#define SN_LINK_KNOWN   0x08

using namespace std;

class SuffixNode
{
  private:
    int id;
    SuffixNode *suffixLink;
    SuffixNode *parent;
    string parentEdgeLabel;
    vector<SuffixNode *> children;
    int stringDepth;
    int flags;

  public:
    SuffixNode(int id);

    //manage parent info
    void setParent(SuffixNode *p);
    SuffixNode *getParent();

    void setEdgeLabel(string label);
    string getEdgeLabel();

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