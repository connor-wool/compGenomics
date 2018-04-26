#include "suffixTree.h"

SuffixTree::SuffixTree()
{
    _root = nullptr;
    _numNodes = 0;
    _numLeaves = 0;
    _numInternal = 0;
    _source = "";
    _alphabet = "";
}

SuffixNode *SuffixTree::findPath(SuffixNode *n, SP section, int suffixNumber)
{
    //cout << "Find Path" << endl;
    //for storing the next node to visit
    SuffixNode *nextHop = nullptr;

    //create a string to describe path, and get first character
    string path = _source.substr(section.start, section.length);
    char firstInPath = path[0];

    //search n's children for an edge potentially matching path
    for (SuffixNode *child : n->getChildren())
    {
        char firstInChild = child->label().at(0);
        if (firstInChild == firstInPath)
        {
            nextHop = child;
            break;
        }
    }

    /* base case: the current node does not have a child that continues our
    insertion string. We create a new leaf node in dynamic memory, and make it
    as a child of the current node. */
    if (nextHop == nullptr)
    {
        SuffixNode *newChild = new SuffixNode(&_source);
        newChild->isLeaf(true);
        newChild->setLabel(section.start, section.length);
        newChild->setParent(n);
        newChild->ID(suffixNumber);
        newChild->depth(n->depth() + section.length);
        n->addChild(newChild);
        _numNodes++;
        _numLeaves++;
        return newChild;
    }

    /* recursion case: at least a partial string match exists for the string
    we want to insert. We compare the path string to this child's edge label 
    until they differ, or until we reach another node.*/
    else
    {
        string childLabel = nextHop->label();

        //cout << "Path: " << path << endl;
        //`cout << "Chld: " << childLabel << endl;

        for (int i = 0; i < childLabel.length(); i++)
        {
            if (childLabel[i] != path[i])
            {
                //cout << "Mismatch at index " << i << endl;
                /* If strings no longer match, create a new internal node that 
                is the new parent of both the nextHop node, and the new leaf 
                node. The leaf node will contain the remainder of the path that
                wasn't matched. */

                //initialize new nodes
                SuffixNode *newInternal = new SuffixNode(&_source);
                SuffixNode *newLeaf = new SuffixNode(&_source);

                //basic info about new nodes
                newInternal->isInternal(true);
                newLeaf->isLeaf(true);
                newLeaf->ID(suffixNumber);

                //set edge labels for new nodes and nextHop
                newInternal->setLabel(section.start, i);
                newLeaf->setLabel(section.start + i, section.length - i);
                nextHop->trimLabelFront(i);

                //cout << "NEWI: " << newInternal->label() << endl;;
                //cout << "NEWL: " << newLeaf->label() << endl;
                //cout << "NHOP: " << nextHop->label() << endl;

                //set new string depth values
                newInternal->depth(n->depth() + i);
                newLeaf->depth(newInternal->depth() + (section.length - i));

                //set parent child relationships
                n->removeChild(nextHop);
                n->addChild(newInternal);
                newInternal->setParent(n);

                newInternal->addChild(nextHop);
                nextHop->setParent(newInternal);

                newInternal->addChild(newLeaf);
                newLeaf->setParent(newInternal);

                _numNodes += 2;
                _numInternal += 1;
                _numLeaves += 1;

                return newLeaf;
            }
        }
        //if we reach this point, the child node was an exact match of path.
        //we will have to recursively search the next node.
        SP nextSection;
        nextSection.start = section.start + childLabel.length();
        nextSection.length = section.length - childLabel.length();
        return findPath(nextHop, nextSection, suffixNumber);
    }
}

SuffixNode *SuffixTree::nodeHops(SuffixNode *n, SP section)
{
    if (section.length == 0)
    {
        return n;
    }

    string beta = _source.substr(section.start, section.length);
    char betaFirstChar = beta[0];
    SuffixNode *nextHop = nullptr;

    for (SuffixNode *child : n->getChildren())
    {
        char childFirstChar = child->label().at(0);
        if (childFirstChar == betaFirstChar)
        {
            nextHop = child;
            break;
        }
    }

    int betaLen = beta.length();
    int nextLen = nextHop->label().length();

    //beta exactly ends at the nextHop node
    if (betaLen == nextLen)
    {
        return nextHop;
    }

    //beta continues past the nextHop node, so recurse
    else if (betaLen > nextLen)
    {
        SP nextSection;
        nextSection.start = section.start + nextLen;
        nextSection.length = section.length - nextLen;
        return nodeHops(nextHop, nextSection);
    }

    //beta ends between n and the nexthop node, make a new internal node, set
    //the relationships, and return the new node.
    else
    {
        SuffixNode *newInternal = new SuffixNode(&_source);
        newInternal->isInternal(true);

        //adjust labels
        newInternal->setLabel(section.start, betaLen);
        nextHop->trimLabelFront(betaLen);
        //n->trimLabelBack(nextLen - betaLen);

        //set depths
        newInternal->depth(n->depth() + betaLen);

        //set parent-child relationships
        n->removeChild(nextHop);
        n->addChild(newInternal);
        newInternal->setParent(n);

        newInternal->addChild(nextHop);
        nextHop->setParent(newInternal);

        _numNodes += 1;
        _numInternal += 1;

        return newInternal;
    }
}

void SuffixTree::Construct(string input, string alphabet)
{
    if (!verifyAlphabet(input, alphabet))
    {
        cout << "Error in construction, alphabet does not match" << endl;
        return;
    }

    //create the master copy of the source string
    string master = input + "$";
    _source = master;
    _alphabet = alphabet;

    //create the root node
    _root = new SuffixNode(&_source);
    _root->isRoot(true);
    _root->isInternal(true);
    _root->depth(0);
    _root->setParent(_root);
    _root->setSL(_root);
    _numNodes += 1;
    _numInternal += 1;

    SuffixNode *lastLeaf = nullptr;

    int finalIndex = _source.length();

    cout << "Constructing Suffix Tree" << endl;
    for (int startIndex = 0; startIndex < _source.length(); startIndex++)
    {
        if(startIndex % 10000 == 0){
            cout << startIndex << " of " << finalIndex << endl;;
        }
        //determine index for next insertion
        int suffixNumber = startIndex;
        string nextSuffix = _source.substr(startIndex);
        SP nextSection;
        nextSection.start = startIndex;
        nextSection.length = nextSuffix.length();

        //base case to insert first leaf into tree
        if (lastLeaf == nullptr)
        {
            lastLeaf = findPath(_root, nextSection, suffixNumber);
        }

        //if the last leaf's parent has a suffix link, use it
        else if (lastLeaf->getParent()->linkKnown())
        {
            SuffixNode *u = lastLeaf->getParent();
            if (u->isRoot())
            {
                lastLeaf = findPath(_root, nextSection, suffixNumber);
            }
            else
            {
                SuffixNode *v = u->getSL();
                nextSection.start += v->depth();
                nextSection.length -= v->depth();
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }
        }

        //go to grandparent if parent's link is unknown
        else
        {
            SuffixNode *u = lastLeaf->getParent();
            SuffixNode *uprime = u->getParent();

            if (uprime->isRoot())
            {
                SuffixNode *vprime = uprime->getSL();
                SP hopSection;
                hopSection.start = u->getLabel().start + 1;
                hopSection.length = u->getLabel().length - 1;
                SuffixNode *v = nodeHops(vprime, hopSection);
                u->setSL(v);
                nextSection.start += v->depth();
                nextSection.length -= v->depth();
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }

            else
            {
                SuffixNode *vprime = uprime->getSL();
                SP hopSection;
                hopSection.start = u->getLabel().start;
                hopSection.length = u->getLabel().length;
                SuffixNode *v = nodeHops(vprime, hopSection);
                u->setSL(v);
                nextSection.start += v->depth();
                nextSection.length -= v->depth();
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }
        }
    }
    renumberInternals();
}

bool SuffixTree::verifyAlphabet(string input, string alphabet)
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

void SuffixTree::renumberInternals()
{
    int startVal = _numLeaves;
    renumberInternalsHelper(_root, startVal);
}

int SuffixTree::renumberInternalsHelper(SuffixNode *n, int value)
{
    int idVal = value;
    if (n->isInternal())
    {
        n->ID(idVal);
        idVal += 1;
        for (auto nodeptr : n->getChildren())
        {
            idVal = renumberInternalsHelper(nodeptr, idVal);
        }
    }
    return idVal;
}

void SuffixTree::BWT()
{
    bwtHelper(_root, &_source);
}

void SuffixTree::bwtHelper(SuffixNode *n, string *source)
{
    if (n->ID() == 0 && n->isLeaf())
    {
        cout << source->at(source->length() - 1) << endl;
    }
    else if (n->isLeaf())
    {
        cout << source->at(n->ID() - 1) << endl;
    }
    else
    {
        for (auto nodeptr : n->getChildren())
        {
            bwtHelper(nodeptr, source);
        }
    }
}

void SuffixTree::PrintTreeStatistics()
{
    cout << "Connor Wool's Suffix Tree Stats:" << endl;
    cout << "Internal Nodes: " << _numInternal << endl;
    cout << "Leaf Nodes: " << _numLeaves << endl;
    cout << "Total Nodes: " << _numNodes << endl;
    int treeSizeBytes = _numNodes * sizeof(SuffixNode);
    cout << "Size of tree (bytes): " << treeSizeBytes << endl;
    cout << "Size of node (bytes): " << sizeof(SuffixNode) << endl;
}

void SuffixTree::dfsTraverse()
{
    cout << "Traverse" << endl;
    dfsTraverseHelper(_root);
    cout << "------------Finished" << endl;
}

int SuffixTree::dfsTraverseHelper(SuffixNode *n)
{
    cout << "(" << n->ID() << ") " << "[" << n << "] ";
    cout << "(" << n->getLabel().start << "," << n->getLabel().length << ") --> ";
    for (auto childptr : n->getChildren())
    {
        cout << "[" << childptr << "](" << childptr->getLabel().start << ",";
        cout << childptr->getLabel().length << ") ";
    }
    cout << endl;
    for (auto childptr : n->getChildren())
    {
        dfsTraverseHelper(childptr);
    }
}

void SuffixTree::PrepareST()
{
    //_A = (int *)malloc(sizeof(int) * _source.length());
    //_A_size = _source.length();
    _nextIndex = 0;
    PrepareSTRecursive(_root);
}

void SuffixTree::PrepareSTRecursive(SuffixNode *n)
{
    if (n == nullptr)
    {
        return;
    }
    if (n->isLeaf())
    {
        _A.push_back(n->ID());
        if (n->depth() >= _x)
        {
            n->_start_leaf_index = _nextIndex;
            n->_end_leaf_index = _nextIndex;
        }
        _nextIndex++;
        return;
    }
    if (n->isInternal())
    {
        for (auto c : n->getChildren())
        {
            PrepareSTRecursive(c);
        }
        if (n->depth() >= _x)
        {
            SuffixNode *uleft = n->getChildren().at(0);
            SuffixNode *uright = n->getChildren().back();
            n->_start_leaf_index = uleft->_start_leaf_index;
            n->_end_leaf_index = uright->_end_leaf_index;
        }
    }
}

vector<int> SuffixTree::FindLoc(string read)
{
    SuffixNode *T = _root;
    SuffixNode *next = nullptr;
    SuffixNode *u = nullptr;
    int read_ptr = 0;

    //track deepest node visited
    int deepestDepth = 0;
    SuffixNode *deepestNode = nullptr;

label_restart_while:
    while (read_ptr < read.length())
    {
        //cout << "depth: " << T->depth() << endl;
        next = nullptr;

        for (SuffixNode *child : T->getChildren())
        {
            if (child->label().at(0) == read[read_ptr])
            {
                next = child;
                break;
            }
        }

        if (next == nullptr)
        {
            //cout << "findloc: next is nullptr, mismatch case A" << endl;
            u = T;
            if (u->depth() > deepestDepth && u->depth() >= _x)
            {
                deepestDepth = u->depth();
                deepestNode = u;
            }
            T = u->getSL();
            goto label_restart_while;
        }

        string comp = next->label();

        for (int i = 0; i < comp.length(); i++)
        {
            if (read[read_ptr] != comp[i])
            {
                read_ptr -= i;
                u = T;
                if (u->depth() > deepestDepth && u->depth() >= _x)
                {
                    deepestDepth = u->depth();
                    deepestNode = u;
                }
                T = u->getSL();
                goto label_restart_while;
            }
            read_ptr++;
        }
        T = next;
    }

    vector<int> results;
    if (deepestNode != nullptr)
    {
        //cout << "deepest node not null!" << endl;
        //cout << "deepest node start: " << deepestNode->_start_leaf_index << " end: " << deepestNode->_end_leaf_index << endl;
        for (int i = deepestNode->_start_leaf_index; i <= deepestNode->_end_leaf_index; i++)
        {
            results.push_back(_A[i]);
        }
    }
    return results;
}
