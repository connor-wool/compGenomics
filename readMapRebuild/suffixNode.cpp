#include "suffixNode.h"

//takes a pointer to the given struct, and initializes it.
void initNode(NODE *n)
{
    memset(n, 0, sizeof(NODE));
    //memset(n->children, 0, sizeof(n->children));
    n->id = -1;
    n->edge.start = -1;
    n->edge.length = -1;
    //n->parent = 0;
    //n->children = (NODE*)malloc(6*sizeof(NODE*));
    //n->suffixLink = 0;
    //n->numChildren = 0;
    n->childArrSize = 6;
    n->startLeafIndex = -1;
    n->endLeafIndex = -1;
    n->depth = -1; 
    //n->isInternal = false;
    //n->isLeaf = false;
    //n->isRoot = false;
}

//add a child to the current node, in alpha order
void addChild(char *sourceString, NODE *parent, NODE *child)
{
    char insert = sourceString[child->edge.start];
    for (int i = 0; i < parent->numChildren; i++)
    {
        char compare = sourceString[parent->children[i]->edge.start];
        if (insert < compare)
        {
            //move contents of array over
            for(int j = parent->numChildren; j > i; j--){
                parent->children[j] = parent->children[j-1];
            }
            parent->children[i] = child;
            parent->numChildren++;
            return;
        }
    }
    parent->children[parent->numChildren] = child;
    parent->numChildren++;
}

void removeChild(SuffixNode *parent, SuffixNode *toRemove)
{
    for (int i = 0; i < parent->numChildren; i++)
    {
        if (parent->children[i] == toRemove)
        {
            parent->children[i] = 0;
            parent->numChildren--;
            for (int j = i; j < parent->numChildren; j++)
            {
                parent->children[j] = parent->children[j + 1];
            }
            parent->children[parent->numChildren] = 0;
            return;
        }
    }
}

void trimLabelFront(NODE *n, int howMuch){
    n->edge.start += howMuch;
    n->edge.length -= howMuch;
}

void trimLabelBack(NODE *n, int howMuch)
{
    n->edge.length -= howMuch;
}