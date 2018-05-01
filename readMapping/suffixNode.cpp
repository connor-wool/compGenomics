#include "suffixNode.h"

//All values set to -1
//requires a pointer to the source string
SuffixNode::SuffixNode(std::string *source)
{
    _source = source;
    _id = -1;
    _suffixLink = nullptr;
    _parent = nullptr;
    _edge.start = -1;
    _edge.length = -1;
    _stringDepth = -1;
    _isInternal = false;
    _isRoot = false;
    _isLeaf = false;
    _start_leaf_index = -1;
    _end_leaf_index = -1;
}

void SuffixNode::setParent(SuffixNode *p)
{
    _parent = p;
}

SuffixNode *SuffixNode::getParent()
{
    return _parent;
}

//add a child to the current node, in alpha order
void SuffixNode::addChild(SuffixNode *n)
{
    char nFirst = (*_source)[_edge.start];
    for (int i = 0; i < _children.size(); i++)
    {
        SuffixNode *child = _children[i];
        char cFirst = (*_source)[child->_edge.start];
        if (nFirst < cFirst)
        {
            _children.insert(_children.begin() + i, n);
            return;
        }
    }
    _children.push_back(n);
}

//remove a node from the current list of nodes
void SuffixNode::removeChild(SuffixNode *n)
{
    for (int i = 0; i < _children.size(); i++)
    {
        if (_children[i] == n)
        {
            _children.erase(_children.begin() + i);
            return;
        }
    }
}

//return the current vector of children
std::vector<SuffixNode *> SuffixNode::getChildren()
{
    return _children;
}

//edge label management
void SuffixNode::setLabel(int start, int length)
{
    if (start > -1 && length > 0)
    {
        _edge.start = start;
        _edge.length = length;
    }
    else
    {
        std::cout << "SuffixNode::SetLabel:Error: ";
        std::cout << start << " " << length;
        std::cout << " invalid params" << std::endl;
    }
}

SP SuffixNode::getLabel()
{
    return _edge;
}

void SuffixNode::trimLabelFront(int howMuch){
    _edge.start += howMuch;
    _edge.length -= howMuch;
}

void SuffixNode::trimLabelBack(int howMuch)
{
    _edge.length -= howMuch;
}

void SuffixNode::setSL(SuffixNode *n)
{
    _suffixLink = n;
}

SuffixNode *SuffixNode::getSL()
{
    return _suffixLink;
}

bool SuffixNode::linkKnown()
{
    if (_suffixLink == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void SuffixNode::ID(int id)
{
    _id = id;
}

int SuffixNode::ID()
{
    return _id;
}

void SuffixNode::depth(int d)
{
    _stringDepth = d;
}

int SuffixNode::depth()
{
    return _stringDepth;
}

void SuffixNode::isRoot(bool value)
{
    _isRoot = value;
}

bool SuffixNode::isRoot()
{
    return _isRoot;
}

void SuffixNode::isInternal(bool value)
{
    _isInternal = value;
}

bool SuffixNode::isInternal()
{
    return _isInternal;
}

void SuffixNode::isLeaf(bool value)
{
    _isInternal = !value;
}

bool SuffixNode::isLeaf()
{
    return !_isInternal;
}

std::string SuffixNode::label()
{
    std::string eLabel = _source->substr(_edge.start, _edge.length);
    return eLabel;
}