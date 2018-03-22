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
SuffixNode* SuffixTree::FindPath(SuffixNode *start, string s, int suffixNumber)
{
    cout << "FINDPATH" << endl;
    /* First, we search the children of the current node, looking for a pathlabel
    starting with the same character as the suffix we want to insert */
    SuffixNode *nextHop = nullptr;
    for (SuffixNode *child : start->getChildren())
    {
        char firstChildChar = child->getEdgeLabel().at(0);
        if (firstChildChar == s[0])
        {
            nextHop = child;
        }
    }

    /* base case: the starting node does not have a child that continues our
    insertion string. We create a new leaf node in the heap, and add a pointer
    to the new node in the children of the start node */
    if (nextHop == nullptr)
    {
        cout << "FINDPATH: NEXTHOP=NULLPTR" << endl;
        SuffixNode *newChild = new SuffixNode();
        newChild->setAsLeaf();
        newChild->setEdgeLabel(s);
        newChild->setParent(start);
        newChild->setId(suffixNumber);
        newChild->setStringDepth(start->getStringDepth() + s.length());
        newChild->setParent(start);
        start->addChild(newChild);
        this->number_nodes += 1;
        this->number_leaf_nodes += 1;
        return newChild;
    }

    /* recursion case: at least a partial string match exists for the string
    we want to insert. Follow it down either until the string does not match,
    or until we get to another node. */
    else
    {
        cout << "FINDPATH: NEXTHOP=" << nextHop->getEdgeLabel() << endl;
        string comparisonString = nextHop->getEdgeLabel();
        cout << "FINDPATH: cmp string: " << comparisonString << endl;
        cout << "FINDPATH: ins string: " << s << endl;
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
                cout << "FINDPATH: comparison string " << comparisonString << endl;
                cout << "FINDPATH: tophalf: " << topHalfString << endl;
                cout << "FINDPATH: bottomhalf: " << bottomHalfString << endl;
                cout << "FINDPATH: s string " << s << endl;
                cout << "FINDPATH: newLeafString " << newLeafString << endl;

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
                return newLeaf;
            }
        }
        return FindPath(nextHop, s.substr(comparisonString.length()), suffixNumber);
    }
}

SuffixNode* SuffixTree::NodeHops(SuffixNode *start, string beta)
{
    //do a thing? The thing? This thing?

    if(beta.length() == 0){
        return start;
    }

    SuffixNode *nextHop = nullptr;
    for (SuffixNode *child : start->getChildren())
    {
        char firstChildChar = child->getEdgeLabel().at(0);
        if (firstChildChar == beta[0])
        {
            nextHop = child;
        }
    }

    /* if beta is exhausted by traveling to nextHop, just return nextHop */
    if(beta.length() == nextHop->getEdgeLabel().length()){
        return nextHop;
    }

    /* if beta is longer than the path to nextHop, recurse */
    else if(beta.length() > nextHop->getEdgeLabel().length()){
        return NodeHops(nextHop, beta.substr(nextHop->getEdgeLabel().size()));
    }

    /* if beta ends in the middle of a path label, make a new internal node and
    return it */
    else{
        string topHalf = nextHop->getEdgeLabel().substr(0, beta.length());
        string bottomHalf = nextHop->getEdgeLabel().substr(beta.length());

        SuffixNode *newInternalNode = new SuffixNode();
        newInternalNode->setAsInternal();

        newInternalNode->setEdgeLabel(topHalf);
        newInternalNode->setStringDepth(start->getStringDepth() + topHalf.length());
        nextHop->setEdgeLabel(bottomHalf);

        newInternalNode->addChild(nextHop);
        start->removeChild(nextHop);
        start->addChild(newInternalNode);

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
        cout << "CONSTRUCT: Input string and alphabet verified" << endl;
    }
    else
    {
        cout << "CONSTRUCT: ERROR: input string not in alphabet" << endl;
        exit(1);
    }

    //add $ to end of input string
    string master = input + "$";

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
        cout << "CONSTRUCT: nextSuffix is `" << nextSuffix << "`" << endl;

        //initialization, insert the first suffix into the tree
        if(lastLeaf == nullptr){
            lastLeaf = FindPath(this->root, nextSuffix, loopControl);
        }
        //case when suffix link is known for parent
        else if(lastLeaf->getParent()->linkKnown()){
            //get parent node
            SuffixNode *u = lastLeaf->getParent();
            //if parent is root, insert starting at root (no savings)
            if(u->isRoot()){
                lastLeaf = FindPath(this->root, nextSuffix, loopControl);
            }
            //if parent is not root, follow suffix link and insert
            else{
                string toInsert = nextSuffix.substr(u->getStringDepth());
                lastLeaf = FindPath(u->getSuffixLink(), toInsert, loopControl);
            }
        }
        //case when suffix link is not known for parent
        else{
            //get grandparent node
            SuffixNode *uprime = lastLeaf->getParent()->getParent();
            SuffixNode *u = lastLeaf->getParent();
            
            if (uprime->isRoot())   //do root stuff
            {
                SuffixNode *vprime = uprime->getSuffixLink();
                SuffixNode *v = NodeHops(vprime, u->getEdgeLabel().substr(1));
                u->setSuffixLink(v);
                u->setLinkKnown();
                string toInsert = nextSuffix.substr(u->getEdgeLabel().size() - 1);
                lastLeaf = FindPath(v, toInsert, loopControl);
            }
            else{   //do not-root stuff
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

    dfsTraverseHelper(this->root);
}

void SuffixTree::dfsTraverseHelper(SuffixNode *start)
{
    //print self, then children
    start->printNode();
    for(auto nodePtr : start->getChildren()){
        dfsTraverseHelper(nodePtr);
    }
}

    void SuffixTree::renumberInternals(){
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
            for(auto nodePtr : start->getChildren()){
                idVal = renumberInternalsHelper(nodePtr, idVal);
            }
        }
        return idVal;
    }

