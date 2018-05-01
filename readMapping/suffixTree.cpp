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
    SuffixNode *nextHop = nullptr;

    char *path = &_source[section.start];
    char firstInPath = _source[section.start];

    //search n's children for an edge potentially matching path
    for (SuffixNode *child : n->_children)//getChildren())
    {
        char firstInChild = _source[child->_edge.start];
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
        newChild->_isLeaf = true;
        newChild->_edge.start = section.start;
        newChild->_edge.length = section.length;
        newChild->_parent = n;
        newChild->_id = suffixNumber;
        newChild->_stringDepth = n->_stringDepth + section.length;
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
        char *childLabel = &_source[nextHop->_edge.start];

        for (int i = 0; i < nextHop->_edge.length; i++)
        {
            if (childLabel[i] != path[i])
            {
                /* If strings no longer match, create a new internal node that 
                is the new parent of both the nextHop node, and the new leaf 
                node. The leaf node will contain the remainder of the path that
                wasn't matched. */

                //initialize new nodes
                SuffixNode *newInternal = new SuffixNode(&_source);
                SuffixNode *newLeaf = new SuffixNode(&_source);

                //basic info about new nodes
                newInternal->_isInternal = true;
                newLeaf->_isLeaf = true;
                newLeaf->_id = suffixNumber;

                //set edge labels for new nodes and nextHop
                newInternal->_edge.start = section.start;
                newInternal->_edge.length = i;
                newLeaf->_edge.start = section.start + i;
                newLeaf->_edge.length = section.length - i;
                nextHop->trimLabelFront(i);

                //set new string depth values
                newInternal->_stringDepth = n->_stringDepth + i;
                newLeaf->_stringDepth = newInternal->_stringDepth + (section.length - i);

                //set parent child relationships
                n->removeChild(nextHop);
                n->addChild(newInternal);
                newInternal->_parent = n;

                newInternal->addChild(nextHop);
                nextHop->_parent = newInternal;

                newInternal->addChild(newLeaf);
                newLeaf->_parent = newInternal;

                _numNodes += 2;
                _numInternal += 1;
                _numLeaves += 1;

                return newLeaf;
            }
        }
        //if we reach this point, the child node was an exact match of path.
        //we will have to recursively search the next node.
        SP nextSection;
        nextSection.start = section.start + nextHop->_edge.length;
        nextSection.length = section.length - nextHop->_edge.length;
        return findPath(nextHop, nextSection, suffixNumber);
    }
}

SuffixNode *SuffixTree::nodeHops(SuffixNode *n, SP section)
{
    if (section.length == 0)
    {
        return n;
    }

    char *beta = &_source[section.start];
    char betaFirstChar = _source[section.start];

    SuffixNode *nextHop = nullptr;

    for (SuffixNode *child : n->_children)
    {
        char childFirstChar = _source[child->_edge.start];
        if (childFirstChar == betaFirstChar)
        {
            nextHop = child;
            break;
        }
    }

    int betaLen = section.length;
    int nextLen = nextHop->_edge.length;

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
        newInternal->_isInternal = true;

        //adjust labels
        newInternal->_edge.start = section.start;
        newInternal->_edge.length = betaLen;
        nextHop->trimLabelFront(betaLen);

        //set depths
        newInternal->_stringDepth = n->_stringDepth + betaLen;

        //set parent-child relationships
        n->removeChild(nextHop);
        n->addChild(newInternal);
        newInternal->_parent = n;

        newInternal->addChild(nextHop);
        nextHop->_parent = newInternal;

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
        //determine index for next insertion
        int suffixNumber = startIndex;

        SP nextSection;
        nextSection.start = startIndex;
        nextSection.length = finalIndex - startIndex;

        //base case to insert first leaf into tree
        if (lastLeaf == nullptr)
        {
            lastLeaf = findPath(_root, nextSection, suffixNumber);
        }

        //if the last leaf's parent has a suffix link, use it
        else if(lastLeaf->_parent->_suffixLink != nullptr)
        {
            SuffixNode *u = lastLeaf->_parent;
            if (u->_isRoot)
            {
                lastLeaf = findPath(_root, nextSection, suffixNumber);
            }
            else
            {
                SuffixNode *v = u->_suffixLink;
                nextSection.start += v->_stringDepth;
                nextSection.length -= v->_stringDepth;
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }
        }

        //go to grandparent if parent's link is unknown
        else
        {
            SuffixNode *u = lastLeaf->_parent;
            SuffixNode *uprime = u->_parent;

            if (uprime->_isRoot)
            {
                SuffixNode *vprime = uprime->_suffixLink;
                SP hopSection;
                hopSection.start = u->getLabel().start + 1;
                hopSection.length = u->getLabel().length - 1;
                SuffixNode *v = nodeHops(vprime, hopSection);
                u->_suffixLink = v;
                nextSection.start += v->_stringDepth;
                nextSection.length -= v->_stringDepth;
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }

            else
            {
                SuffixNode *vprime = uprime->_suffixLink;
                SP hopSection;
                hopSection.start = u->_edge.start;
                hopSection.length = u->_edge.length;
                SuffixNode *v = nodeHops(vprime, hopSection);
                u->_suffixLink = v;
                nextSection.start += v->_stringDepth;
                nextSection.length -= v->_stringDepth;
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
    _nextIndex = 0;
    PrepareSTRecursive(_root);
}

void SuffixTree::PrepareSTRecursive(SuffixNode *n)
{
    if (n == nullptr)
    {
        return;
    }
    if (n->_isLeaf == true)
    {
        _A.push_back(n->_id);
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
        for (auto c : n->_children)
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
        next = nullptr;

        for (SuffixNode *child : T->_children)
        {
            char childChar = _source[child->_edge.start];
            if (childChar == read[read_ptr])
            {
                next = child;
                break;
            }
        }

        if (next == nullptr)
        {
            u = T;
            if (u->_stringDepth > deepestDepth && u->_stringDepth >= _x)
            {
                deepestDepth = u->_stringDepth;
                deepestNode = u;
            }
            T = u->_suffixLink;
            goto label_restart_while;
        }

        char *comp = &_source[next->_edge.start];

        for (int i = 0; i < next->_edge.length; i++)
        {
            if (read[read_ptr] != comp[i])
            {
                read_ptr -= i;
                u = T;
                if (u->_stringDepth > deepestDepth && u->_stringDepth >= _x)
                {
                    deepestDepth = u->_stringDepth;
                    deepestNode = u;
                }
                T = u->_suffixLink;
                goto label_restart_while;
            }
            read_ptr++;
        }
        T = next;
    }

    vector<int> results;
    if (deepestNode != nullptr)
    {
        for (int i = deepestNode->_start_leaf_index; i <= deepestNode->_end_leaf_index; i++)
        {
            results.push_back(_A[i]);
        }
    }
    return results;
}
