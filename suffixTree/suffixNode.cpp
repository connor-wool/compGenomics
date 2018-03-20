/* suffixNode.cpp - describes a data structure for a node in the suffix tree.
Written by Connor Wool, Spring 2018 */

#include "suffixNode.h"

SuffixNode::SuffixNode(int node_id)
{
    this->flags = 0;
    this->id = node_id;
    this->parentEdgeLabel = "";
    this->parent = nullptr;
    this->suffixLink = nullptr;
}

void SuffixNode::setParent(SuffixNode *p)
{
    this->parent = p;
}

SuffixNode *SuffixNode::getParent()
{
    return this->parent;
}

void SuffixNode::setEdgeLabel(string label)
{
    this->parentEdgeLabel = label;
}

string SuffixNode::getEdgeLabel()
{
    return this->parentEdgeLabel;
}

void SuffixNode::setAsRoot()
{
    this->flags = (this->flags | SN_IS_ROOT);
}

bool SuffixNode::isRoot()
{
    return (this->flags & SN_IS_ROOT);
}

void SuffixNode::setAsInternal()
{
    this->flags = (this->flags | SN_IS_INTERNAL);
}

bool SuffixNode::isInternal()
{
    return (this->flags & SN_IS_INTERNAL);
}

void SuffixNode::setAsLeaf()
{
    this->flags = (this->flags | SN_IS_LEAF);
}

bool SuffixNode::isLeaf()
{
    return (this->flags & SN_IS_LEAF);
}

void SuffixNode::setLinkKnown()
{
    this->flags = (this->flags | SN_LINK_KNOWN);
}

void SuffixNode::setLinkUnknown()
{
    this->flags = (this->flags & (~SN_LINK_KNOWN));
}

bool SuffixNode::linkKnown()
{
    return (this->flags & SN_LINK_KNOWN);
}