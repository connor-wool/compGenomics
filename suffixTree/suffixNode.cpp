/* suffixNode.cpp - describes a data structure for a node in the suffix tree.
Written by Connor Wool, Spring 2018 */

#include "suffixNode.h"

SuffixNode::SuffixNode(string *source)
{
    this->flags = 0;
    this->id = -1;
    this->source = source;
    this->parent = nullptr;
    this->suffixLink = nullptr;
    this->setLinkUnknown();
}

SuffixNode::SuffixNode(string *source, int node_id)
{
    this->flags = 0;
    this->id = node_id;
    this->source = source;
    this->parent = nullptr;
    this->suffixLink = nullptr;
}

SuffixNode::SuffixNode(string *source, int id, SuffixNode *parent)
{
    this->id = id;
    this->source = source;
    this->parent = parent;
}

void SuffixNode::setParent(SuffixNode *p)
{
    this->parent = p;
}

SuffixNode *SuffixNode::getParent()
{
    return this->parent;
}

void SuffixNode::setEdgeLabel(int start, int length)
{
    this->el_startIndex = start;
    this->el_length = length;
}

string SuffixNode::getEdgeLabel()
{
    string temp = this->source->substr(this->el_startIndex, this->el_length);
    return temp;
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

void SuffixNode::addChild(SuffixNode *n)
{
    char insertionChar = n->getEdgeLabel().at(0);
    for (int i = 0; i < this->children.size(); i++)
    {
        char childChar = this->children[i]->getEdgeLabel().at(0);
        if (insertionChar < childChar)
        {
            this->children.insert(children.begin() + i, n);
            return;
        }
    }
    this->children.push_back(n);
}

void SuffixNode::removeChild(SuffixNode *n)
{
    for (int i = 0; i < this->children.size(); i++)
    {
        if (this->children[i] == n)
        {
            this->children.erase(this->children.begin() + i);
            return;
        }
    }
}

vector<SuffixNode *> SuffixNode::getChildren()
{
    return this->children;
}

void SuffixNode::setSuffixLink(SuffixNode *n)
{
    this->suffixLink = n;
}

SuffixNode *SuffixNode::getSuffixLink()
{
    return this->suffixLink;
}

void SuffixNode::setId(int id)
{
    this->id = id;
}

int SuffixNode::getId()
{
    return this->id;
}

void SuffixNode::printChildren()
{
    cout << "[";
    for (auto nodePtr : this->children)
    {
        cout << " '" << nodePtr->getEdgeLabel() << "'";
    }
    cout << " ]";
}

void SuffixNode::printNode()
{
    cout << "{ ID:" << this->id;
    cout << " | '" << this->getEdgeLabel() << "'";
    cout << " | D:" << this->stringDepth;

    if (this->isLeaf())
    {
        cout << "| Leaf";
    }
    else
    {
        cout << " | Children: ";
        printChildren();
    }
    cout << " }" << endl;
}

void SuffixNode::setStringDepth(int depth)
{
    this->stringDepth = depth;
}

int SuffixNode::getStringDepth()
{
    return this->stringDepth;
}

void SuffixNode::printDepth()
{
    cout << this->stringDepth << " ";
}

int SuffixNode::getEdgeStart()
{
    return this->el_startIndex;
}

int SuffixNode::getEdgeLength()
{
    return this->el_length;
}