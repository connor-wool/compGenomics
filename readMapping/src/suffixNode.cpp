#include "suffixNode.h"

//All values set to -1
//requires a pointer to the source string
SuffixNode::SuffixNode(std::string *source)
{
    _suffixLink = nullptr;
    _parent = nullptr;
    _source = source;
    //vector needs no initalization
    _id = -1;
    _stringDepth = -1;
    _start_leaf_index = -1;
    _end_leaf_index = -1;
    _edge.start = -1;
    _edge.length = -1;
    _isInternal = false;
    _isRoot = false;
    _isLeaf = false;
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

void SuffixNode::trimLabelFront(int howMuch){
    _edge.start += howMuch;
    _edge.length -= howMuch;
}

void SuffixNode::trimLabelBack(int howMuch)
{
    _edge.length -= howMuch;
}