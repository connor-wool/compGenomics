/* suffixNode.cpp - describes a data structure for a node in the suffix tree.
Written by Connor Wool, Spring 2018 */

#include "suffixNode.h"

SuffixNode::SuffixNode(int node_id){
    this->flags = 0;
    this->id = node_id;
    this->edgeLabel = "";
    this->parent = void;
    this->suffixLink = void;
}