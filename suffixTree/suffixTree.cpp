#include "suffixTree.h"

SuffixTree::SuffixTree()
{
    this->root = nullptr;
    this->number_nodes = 0;
    this->number_leaf_nodes = 0;
    this->number_internal_nodes = 0;
}
/* finds the path starting at the specified node argument that spells out the
longest possible prefix of the specified string argument, and then insert
the next suffix */
SuffixNode *SuffixTree::FindPath(SuffixNode *start, string s, int suffixNumber)
{
    //cout << "FINDPATH ++++++++++++++++++++++++++++++++++++++++ BEGIN" << endl;
    //cout << "Is Start node root? (" << start->isRoot() << ")" << endl;
    //cout << "Is Start's parent root? (" << start->getParent()->isRoot() << ")" << endl;
    //cout << "Start node edge label: " << start->getEdgeLabel() << endl;
    //cout << "inserting: " << s << endl;

    SuffixNode *nextHop = nullptr;
    for (SuffixNode *child : start->getChildren())
    {
        //cout << "Looking at child with edge label: " << child->getEdgeLabel() << endl;
        char firstChildChar = child->getEdgeLabel().at(0);
        if (firstChildChar == s[0])
        {
            nextHop = child;
            break;
        }
    }

    /* base case: the starting node does not have a child that continues our
    insertion string. We create a new leaf node in the heap, and add a pointer
    to the new node in the children of the start node */
    if (nextHop == nullptr)
    {
        //cout << "FINDPATH: nextHop var == nullPtr, creating new child at start" << endl;
        SuffixNode *newChild = new SuffixNode();
        newChild->setAsLeaf();
        newChild->setEdgeLabel(s);
        newChild->setParent(start);
        newChild->setId(suffixNumber);
        newChild->setStringDepth(start->getStringDepth() + s.length());
        newChild->setParent(start);
        start->addChild(newChild);
        //cout << "added child with label: " << newChild->getEdgeLabel() << endl;
        this->number_nodes += 1;
        this->number_leaf_nodes += 1;
        //cout << "FINDPATH ------------------------------------ END" << endl;
        return newChild;
    }

    /* recursion case: at least a partial string match exists for the string
    we want to insert. Follow it down either until the string does not match,
    or until we get to another node. */
    else
    {
        //cout << "FINDPATH: nextHop is child with edgeLabel: " << nextHop->getEdgeLabel() << endl;
        string comparisonString = nextHop->getEdgeLabel();
        //cout << "FINDPATH: cmp string: " << comparisonString << endl;
        //cout << "FINDPATH: ins string: " << s << endl;
        for (int i = 0; i < comparisonString.length(); i++)
        {
            //check for a point where the strings no longer match
            if (comparisonString[i] != s[i])
            {
                /* If there's a point where the strings no longer match,
                we will create a new internal node that is the new parent of
                the nextHop node, and the new leaf node we want to insert with
                the remainder of s that wasn't matched. */

                /* split the old path string where it deviates;
                the top half is the common prefix of the existing node and
                the new node we will insert. The bottom half is the part
                unique to the old path, and we should put that into the
                nextHop node */
                string topHalfString = comparisonString.substr(0, i);
                string bottomHalfString = comparisonString.substr(i);
                string newLeafString = s.substr(i);
                //cout << "FINDPATH: tophalf: " << topHalfString << endl;
                //cout << "FINDPATH: bottomhalf: " << bottomHalfString << endl;
                //cout << "FINDPATH: newLeafString " << newLeafString << endl;

                /* Create the new internal parent node. */
                SuffixNode *newInternalNode = new SuffixNode();
                newInternalNode->setAsInternal();

                /* Create a new leaf node */
                SuffixNode *newLeaf = new SuffixNode();
                newLeaf->setAsLeaf();
                newLeaf->setId(suffixNumber);

                /* Set edge labels appropriately */
                newInternalNode->setEdgeLabel(topHalfString);
                newInternalNode->setStringDepth(start->getStringDepth() + topHalfString.length());
                nextHop->setEdgeLabel(bottomHalfString);
                //nextHop string depth should not change.
                newLeaf->setEdgeLabel(newLeafString);
                newLeaf->setStringDepth(newInternalNode->getStringDepth() + newLeafString.length());

                /* Re-arrange children. Remove child from start, add two
                children to new internal node */
                start->removeChild(nextHop);
                start->addChild(newInternalNode);
                newInternalNode->setParent(start);
                newInternalNode->addChild(nextHop);
                nextHop->setParent(newInternalNode);
                newInternalNode->addChild(newLeaf);
                newLeaf->setParent(newInternalNode);
                this->number_nodes += 2;
                this->number_internal_nodes += 1;
                this->number_leaf_nodes += 1;
                //cout << "FINDPATH -------------------- END" << endl;

                return newLeaf;
            }
        }
        bool areEqual = (s.compare(comparisonString) == 0);
        //cout << "FINDPATH recursing..." << endl;
        return FindPath(nextHop, s.substr(comparisonString.length()), suffixNumber);
    }
}

SuffixNode *SuffixTree::NodeHops(SuffixNode *start, string beta)
{
    //do a thing? The thing? This thing?

    if (beta.length() == 0)
    {
        return start;
    }

    SuffixNode *nextHop = nullptr;
    for (SuffixNode *child : start->getChildren())
    {
        char firstChildChar = child->getEdgeLabel().at(0);
        if (firstChildChar == beta[0])
        {
            nextHop = child;
            break;
        }
    }

    /* if beta is exhausted by traveling to nextHop, just return nextHop */
    if (beta.length() == nextHop->getEdgeLabel().length())
    {
        return nextHop;
    }

    /* if beta is longer than the path to nextHop, recurse */
    else if (beta.length() > nextHop->getEdgeLabel().length())
    {
        return NodeHops(nextHop, beta.substr(nextHop->getEdgeLabel().size()));
    }

    /* if beta ends in the middle of a path label, make a new internal node and
    return it */
    else
    {
        string topHalf = nextHop->getEdgeLabel().substr(0, beta.length());
        string bottomHalf = nextHop->getEdgeLabel().substr(beta.length());

        SuffixNode *newInternalNode = new SuffixNode();
        newInternalNode->setAsInternal();

        newInternalNode->setEdgeLabel(topHalf);
        newInternalNode->setStringDepth(start->getStringDepth() + topHalf.length());
        nextHop->setEdgeLabel(bottomHalf);

        start->addChild(newInternalNode);
        newInternalNode->addChild(nextHop);
        start->removeChild(nextHop);

        newInternalNode->setParent(start);
        nextHop->setParent(newInternalNode);

        this->number_internal_nodes += 1;
        this->number_nodes += 1;

        return newInternalNode;
    }
}

void SuffixTree::Construct(string input, string alphabet)
{

    //verify string is in alphabet
    if (VerifyAlphabet(input, alphabet))
    {
        //cout << "CONSTRUCT: Input string and alphabet verified" << endl;
    }
    else
    {
        //cout << "CONSTRUCT: ERROR: input string not in alphabet" << endl;
        exit(1);
    }

    //add $ to end of input string
    string master = input + "$";
    this->sourceString = master;
    this->sourceAlphabet = alphabet;

    //create a root node
    this->root = new SuffixNode();
    this->root->setAsRoot();
    this->root->setAsInternal();
    this->root->setStringDepth(0);
    this->root->setParent(this->root);
    this->root->setSuffixLink(this->root);
    this->root->setLinkKnown();
    this->number_nodes += 1;
    this->number_internal_nodes += 1;

    SuffixNode *lastLeaf;
    lastLeaf = nullptr;

    for (int loopControl = 0; loopControl < master.length(); loopControl++)
    {
        //create a string to define the next suffix
        string nextSuffix = master.substr(loopControl);
        //cout << "CONSTRUCT: nextSuffix is " << nextSuffix << endl;

        //initialization, insert the first suffix into the tree
        if (lastLeaf == nullptr)
        {
            //cout << "CONSTRUCT: lastLeaf is null, inserting first suffix" << endl;
            lastLeaf = FindPath(this->root, nextSuffix, loopControl);
        }
        //else if(nextSuffix.size() == 1){
        //    lastLeaf = FindPath(this->root, nextSuffix, loopControl);
        //}
        //case when suffix link is known for parent
        else if (lastLeaf->getParent()->linkKnown())
        {
            //get parent node
            SuffixNode *u = lastLeaf->getParent();
            //if parent is root, insert starting at root (no savings)
            if (u->isRoot())
            {
                //cout << "CONSTRUCT: u link known, u is root" << endl;
                lastLeaf = FindPath(this->root, nextSuffix, loopControl);
            }
            //if parent is not root, follow suffix link and insert
            else
            {
                //cout << "CONSTRUCT: u link known, u is not root" << endl;
                SuffixNode *v = u->getSuffixLink();
                string toInsert = nextSuffix.substr(v->getStringDepth());
                lastLeaf = FindPath(v, toInsert, loopControl);
            }
        }
        //case when suffix link is not known for parent
        else
        {
            //get grandparent node
            SuffixNode *u = lastLeaf->getParent();
            SuffixNode *uprime = lastLeaf->getParent()->getParent();

            if (uprime->isRoot()) //do root stuff
            {
                //cout << "CONSTRUCT: u link not known, uprime is root" << endl;
                SuffixNode *vprime = uprime->getSuffixLink();
                SuffixNode *v = NodeHops(vprime, u->getEdgeLabel().substr(1));
                u->setSuffixLink(v);
                u->setLinkKnown();
                string toInsert = nextSuffix.substr(v->getStringDepth());
                lastLeaf = FindPath(v, toInsert, loopControl);
            }
            else
            { //do not-root stuff
                //cout << "CONSTRUCT: u link not known, uprime is not root" << endl;
                SuffixNode *vprime = uprime->getSuffixLink();
                SuffixNode *v = NodeHops(vprime, u->getEdgeLabel());
                u->setSuffixLink(v);
                u->setLinkKnown();
                string toInsert = nextSuffix.substr(v->getStringDepth());
                lastLeaf = FindPath(v, toInsert, loopControl);
            }
        }
    }

    //renumber the internal nodes
    renumberInternals();
}

bool SuffixTree::VerifyAlphabet(string input, string alphabet)
{
    for (char c : input)
    {
        int pos = alphabet.find(c);
        if (pos < 0)
        {
            return false;
        }
    }
    return true;
}

void SuffixTree::dfsTraverse()
{
    cout << "DFS TRAVERSE TREE WITH " << this->number_nodes << " NODES ";
    cout << "(" << this->number_leaf_nodes << " leaves, ";
    cout << this->number_internal_nodes << " internal)" << endl;

    dfsTraverseHelper(this->root, 0);
    cout << endl;
}

int SuffixTree::dfsTraverseHelper(SuffixNode *start, int lineCtl)
{
    //print self, then children
    start->printDepth();
    if (lineCtl + 1 % 10 == 0)
    {
        cout << endl;
    }
    for (auto nodePtr : start->getChildren())
    {
        lineCtl = dfsTraverseHelper(nodePtr, lineCtl + 1);
    }
    return lineCtl;
}

void SuffixTree::renumberInternals()
{
    int startVal = this->number_leaf_nodes;
    renumberInternalsHelper(root, startVal);
}

int SuffixTree::renumberInternalsHelper(SuffixNode *start, int value)
{
    int idVal = value;
    if (start->isInternal())
    {
        start->setId(idVal);
        idVal += 1;
        for (auto nodePtr : start->getChildren())
        {
            idVal = renumberInternalsHelper(nodePtr, idVal);
        }
    }
    return idVal;
}

void SuffixTree::BWT()
{
    //cout << "BWT Index Print:" << endl;
    BWTHelper(this->root, this->sourceString);
    //cout << endl;
}

void SuffixTree::BWTHelper(SuffixNode *start, string source)
{
    if (start->isLeaf() && start->getId() > 0)
    {
        cout << source[start->getId() - 1] << endl;
    }
    else if (start->getId() == 0)
    {
        cout << source.at(source.size() - 1) << endl;
    }
    else
    {
        for (auto nodePtr : start->getChildren())
        {
            BWTHelper(nodePtr, source);
        }
    }
}

void SuffixTree::PrintTreeStatistics()
{
    cout << "CONNOR WOOL'S SUFFIX TREE STATS:" << endl;
    //project requested statistics, in order of request
    cout << "Internal Nodes: " << this->number_internal_nodes << endl;
    cout << "Leaf Nodes: " << this->number_leaf_nodes << endl;
    cout << "Total Nodes: " << this->number_nodes << endl;
    int treeSizeBytes = this->number_nodes * sizeof(SuffixNode);
    cout << "Size of tree (bytes): " << treeSizeBytes << endl;
    //avg string depth, internal nodes
    double avgDepth = (double)sumInternalNodeDepth(this->root) / (double)this->number_internal_nodes;
    cout << "Avg Internal Node String Depth: " << avgDepth << endl;
    //max string depth, internal nodes
    cout << "Max Internal Node String Depth: " << findMaxInternalNodeDepth(this->root) << endl;

    //int inputSizeBytes = this->sourceString.size();
    //cout << "Size of input (bytes): " << inputSizeBytes << " bytes" << endl;
    //cout << "Size of each node (bytes): " << sizeof(SuffixNode) << endl;
    //cout << "Ratio: " << ((double)treeSizeBytes / (double)inputSizeBytes) << endl;
    //cout << "Total Nodes: " << this->number_nodes << endl;
}

int SuffixTree::sumInternalNodeDepth(SuffixNode *start)
{
    int value = 0;
    if (start->isInternal())
    {
        for (auto nodePtr : start->getChildren())
        {
            value += sumInternalNodeDepth(nodePtr);
        }
        value += start->getStringDepth();
    }
    return value;
}

int SuffixTree::findMaxInternalNodeDepth(SuffixNode *start)
{
    int max = 0;
    if (start->isInternal())
    {
        max = start->getStringDepth();
        for (auto nodePtr : start->getChildren())
        {
            int childVal = findMaxInternalNodeDepth(nodePtr);
            if (childVal > max)
            {
                max = childVal;
            }
        }
    }
    return max;
}