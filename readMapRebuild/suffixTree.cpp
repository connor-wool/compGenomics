#include "suffixTree.h"

SuffixTree::SuffixTree()
{
    root = 0;
    _numNodes = 0;
    _numLeaves = 0;
    _numInternal = 0;
    _source = "";
    _alphabet = "";
    _x = 25;
    _nextIndex = 0;
}

NODE *SuffixTree::findPath(NODE *n, SP section, int suffixNumber)
{
    NODE *nextHop = 0;

    //create a string to describe path, and get first character
    //string path = _source.substr(section.start, section.length);
    char *path = &_source[section.start];
    char firstInPath = path[0];

    //search n's children for an edge potentially matching path
    //getChildren())
    for(int i = 0; i < n->numChildren; i++)
    {
        char firstInChild = _source[n->children[i]->edge.start];
        if (firstInChild == firstInPath)
        {
            nextHop = n->children[i];
            break;
        }
    }

    /* base case: the current node does not have a child that continues our
    insertion string. We create a new leaf node in dynamic memory, and make it
    as a child of the current node. */
    if (nextHop == 0)
    {
        NODE *newChild = new NODE;
        initNode(newChild);
        newChild->isLeaf = true;
        newChild->edge.start = section.start;
        newChild->edge.length = section.length;
        newChild->parent = n;
        newChild->id = suffixNumber;
        newChild->depth = (n->depth + section.length);
        addChild(cstring, n, newChild);
        //n->addChild(newChild);
        _numNodes++;
        _numLeaves++;
        return newChild;
    }

    /* recursion case: at least a partial string match exists for the string
    we want to insert. We compare the path string to this child's edge label 
    until they differ, or until we reach another node.*/
    else
    {
        string childLabel = _source.substr(nextHop->edge.start, nextHop->edge.length);

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
                NODE *newInternal = new NODE;
                NODE *newLeaf = new NODE;

                initNode(newInternal);
                initNode(newLeaf);

                //basic info about new nodes
                newInternal->isInternal = true;
                newLeaf->isLeaf = true;
                newLeaf->id = suffixNumber;

                //set edge labels for new nodes and nextHop
                newInternal->edge.start = section.start;
                newInternal->edge.length = i;
                newLeaf->edge.start = section.start + i;
                newLeaf->edge.length = section.length - i;
                trimLabelFront(nextHop, i);
                //nextHop->trimLabelFront(i);

                //cout << "NEWI: " << newInternal->label() << endl;;
                //cout << "NEWL: " << newLeaf->label() << endl;
                //cout << "NHOP: " << nextHop->label() << endl;

                //set new string depth values
                newInternal->depth = n->depth + i;
                newLeaf->depth = (newInternal->depth + (section.length - i));

                //insert our new internal node above it's children
                removeChild(n, nextHop);
                addChild(cstring, n, newInternal);
                newInternal->parent = n;

                //add nextHop as child of new internal node
                addChild(cstring, newInternal, nextHop);
                nextHop->parent = newInternal;

                //add new leaf as child of new internal node
                addChild(cstring, newInternal, newLeaf);
                newLeaf->parent = newInternal;

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

NODE *SuffixTree::nodeHops(NODE *n, SP section)
{
    if (section.length == 0)
    {
        return n;
    }

    char *beta = &_source[section.start];
    char betaFirstChar = beta[0];
    NODE *nextHop = 0;

    for(int i = 0; i < n->numChildren; i++){
        NODE *temp = n->children[0];
        char childFirstChar = _source[temp->edge.start];
        if(childFirstChar == betaFirstChar){
            nextHop = temp;
            break;
        }
    }

    int betaLen = section.length;
    int nextLen = nextHop->edge.length;

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
        NODE *newInternal = new NODE;
        initNode(newInternal);
        newInternal->isInternal = true;

        //adjust labels
        newInternal->edge.start = section.start;
        newInternal->edge.length = betaLen;
        trimLabelFront(nextHop, betaLen);

        //set depths
        newInternal->depth = (n->depth + betaLen);

        //set parent-child relationships
        removeChild(n, nextHop);
        addChild(cstring, n, newInternal);
        newInternal->parent = n;

        //newInternal->addChild(nextHop);
        addChild(cstring, newInternal, nextHop);
        nextHop->parent = newInternal;

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
    cstring = &_source[0];
    _alphabet = alphabet;

    //create the root node
    root = new NODE;
    initNode(root);
    root->isRoot = true;
    root->isInternal = true;
    root->depth = (0);
    root->parent = root;
    root->suffixLink = root;
    _numNodes += 1;
    _numInternal += 1;

    NODE *lastLeaf = 0;

    int finalIndex = _source.length();
    int sourceLen = _source.length();

    cout << "Constructing Suffix Tree" << endl;
    for (int startIndex = 0; startIndex < sourceLen; startIndex++)
    {
        if(startIndex % 10000 == 0){
            cout << startIndex << " of " << finalIndex << endl;;
        }
        //determine index for next insertion
        int suffixNumber = startIndex;
        //string nextSuffix = _source.substr(startIndex);
        SP nextSection;
        nextSection.start = startIndex;
        nextSection.length = finalIndex - startIndex;
        //nextSection.length = nextSuffix.length();

        //cout << finalIndex - startIndex << " " << nextSuffix.length() << endl;

        //base case to insert first leaf into tree
        if (lastLeaf == 0)
        {
            lastLeaf = findPath(root, nextSection, suffixNumber);
        }

        //if the last leaf's parent has a suffix link, use it
        //else if (lastLeaf->getParent()->linkKnown())
        else if(lastLeaf->parent->suffixLink != 0)
        {
            NODE *u = lastLeaf->parent;
            if (u->isRoot)
            {
                lastLeaf = findPath(root, nextSection, suffixNumber);
            }
            else
            {
                NODE *v = u->suffixLink;
                nextSection.start += v->depth;
                nextSection.length -= v->depth;
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }
        }

        //go to grandparent if parent's link is unknown
        else
        {
            NODE *u = lastLeaf->parent;
            NODE *uprime = u->parent;

            if (uprime->isRoot)
            {
                NODE *vprime = uprime->suffixLink;
                SP hopSection;
                hopSection.start = u->edge.start + 1;//getLabel().start + 1;
                hopSection.length = u->edge.length - 1;//getLabel().length - 1;
                NODE *v = nodeHops(vprime, hopSection);
                u->suffixLink = v;//setSL(v);
                nextSection.start += v->depth;//depth();
                nextSection.length -= v->depth;//depth();
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }

            else
            {
                NODE *vprime = uprime->suffixLink;//getSL();
                SP hopSection;
                hopSection.start = u->edge.start;//getLabel().start;
                hopSection.length = u->edge.length;//getLabel().length;
                NODE *v = nodeHops(vprime, hopSection);
                u->suffixLink = v;//setSL(v);
                nextSection.start += v->depth;//depth();
                nextSection.length -= v->depth;//depth();
                lastLeaf = findPath(v, nextSection, suffixNumber);
            }
        }
    }
    renumberInternals();
}

bool SuffixTree::verifyAlphabet(string input, string alphabet)
{
    /*
    for (char c : input)
    {
        int pos = alphabet.find(c);
        if (pos < 0)
        {
            return false;
        }
    }
    */

    for(int i = 0; i < input.length(); i++){
        int pos = alphabet.find(input[i]);
        if(pos < 0){
            return false;
        }
    }
    return true;
}

void SuffixTree::renumberInternals()
{
    int startVal = _numLeaves;
    renumberInternalsHelper(root, startVal);
}

int SuffixTree::renumberInternalsHelper(NODE *n, int value)
{
    int idVal = value;
    if (n->isInternal)
    {
        n->id = idVal;
        idVal += 1;
        for(int i = 0; i < n->numChildren; i++){
            idVal = renumberInternalsHelper(n->children[i], idVal);
        }
    }
    return idVal;
}

void SuffixTree::BWT()
{
    bwtHelper(root, &_source);
}

void SuffixTree::bwtHelper(NODE *n, string *source)
{
    if (n->id == 0 && n->isLeaf)
    {
        cout << source->at(source->length() - 1) << endl;
    }
    else if (n->isLeaf)
    {
        cout << source->at(n->id - 1) << endl;
    }
    else
    {
        for (int i = 0; i < n->numChildren; i++){
            bwtHelper(n->children[i], source);
        }
    }
}

void SuffixTree::PrintTreeStatistics()
{
    cout << "Connor Wool's Suffix Tree Stats:" << endl;
    cout << "Internal Nodes: " << _numInternal << endl;
    cout << "Leaf Nodes: " << _numLeaves << endl;
    cout << "Total Nodes: " << _numNodes << endl;
    int treeSizeBytes = _numNodes * sizeof(NODE);
    cout << "Size of tree (bytes): " << treeSizeBytes << endl;
    cout << "Size of node (bytes): " << sizeof(NODE) << endl;
}

void SuffixTree::dfsTraverse()
{
    cout << "Traverse" << endl;
    dfsTraverseHelper(root);
    cout << "------------Finished" << endl;
}

int SuffixTree::dfsTraverseHelper(NODE *n)
{
    cout << "(" << n->id << ") " << "[" << n << "] ";
    cout << "(" << n->edge.start << "," << n->edge.length << ") --> ";
    for (int i = 0; i < n->numChildren; i++){
        NODE *childptr = n->children[i];
        cout << "[" << childptr << "](" << childptr->edge.start << ",";
        cout << childptr->edge.length << ") ";
    }
    cout << endl;
    for (int i = 0; i < n->numChildren; i++){
        dfsTraverseHelper(n->children[i]);
    }
}

void SuffixTree::PrepareST()
{
    //_A = (int *)malloc(sizeof(int) * _source.length());
    //_A_size = _source.length();
    _nextIndex = 0;
    PrepareSTRecursive(root);
}

void SuffixTree::PrepareSTRecursive(NODE *n)
{
    if (n == 0)
    {
        return;
    }
    if (n->isLeaf)
    {
        _A.push_back(n->id);
        if (n->depth >= _x)
        {
            n->startLeafIndex = _nextIndex;
            n->endLeafIndex = _nextIndex;
        }
        _nextIndex++;
        return;
    }
    if (n->isInternal)
    {
        for(int i = 0; i < n->numChildren; i++){
            PrepareSTRecursive(n->children[i]);
        }
        if (n->depth >= _x)
        {
            NODE *uleft = n->children[0];
            NODE *uright = n->children[n->numChildren-1];
            n->startLeafIndex = uleft->startLeafIndex;
            n->endLeafIndex = uright->endLeafIndex;
        }
    }
}

vector<int> SuffixTree::FindLoc(string read)
{
    SuffixNode *T = root;
    SuffixNode *next = 0;
    SuffixNode *u = 0;
    int read_ptr = 0;

    //track deepest node visited
    int deepestDepth = 0;
    SuffixNode *deepestNode = 0;

label_restart_while:
    while (read_ptr < read.length())
    {
        //cout << "depth: " << T->depth() << endl;
        next = 0;

        for(int i = 0; i < T->numChildren; i++){
            NODE *child = T->children[i];
            if (_source[child->edge.start] == read[read_ptr])
            {
                next = child;
                break;
            }
        }

        if (next == 0)
        {
            //cout << "findloc: next is nullptr, mismatch case A" << endl;
            u = T;
            if (u->depth > deepestDepth && u->depth >= _x)
            {
                deepestDepth = u->depth;
                deepestNode = u;
            }
            T = u->suffixLink;
            goto label_restart_while;
        }

        string comp = _source.substr(next->edge.start, next->edge.length);

        for (int i = 0; i < comp.length(); i++)
        {
            if (read[read_ptr] != comp[i])
            {
                read_ptr -= i;
                u = T;
                if (u->depth > deepestDepth && u->depth >= _x)
                {
                    deepestDepth = u->depth;
                    deepestNode = u;
                }
                T = u->suffixLink;
                goto label_restart_while;
            }
            read_ptr++;
        }
        T = next;
    }

    vector<int> results;
    if (deepestNode != 0)
    {
        //cout << "deepest node not null!" << endl;
        //cout << "deepest node start: " << deepestNode->_start_leaf_index << " end: " << deepestNode->_end_leaf_index << endl;
        for (int i = deepestNode->startLeafIndex; i <= deepestNode->endLeafIndex; i++)
        {
            results.push_back(_A[i]);
        }
    }
    return results;
}
