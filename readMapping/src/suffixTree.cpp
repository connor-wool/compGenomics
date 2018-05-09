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
    //for (SuffixNode *child : n->_children)
    for(int i = 0; i < n->_children.size(); i++)
    {
        char firstInChild = _source[n->_children[i]->_edge.start];
        if (firstInChild == firstInPath)
        {
            nextHop = n->_children[i];
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

    //for (SuffixNode *child : n->_children)
    for(int i = 0; i < n->_children.size(); i++)
    {
        char childFirstChar = _source[n->_children[i]->_edge.start];
        if (childFirstChar == betaFirstChar)
        {
            nextHop = n->_children[i];
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
    cout << "Constructing Suffix Tree" << endl;
    if (!verifyAlphabet(input, alphabet))
    {
        cout << "Error in construction, alphabet does not match" << endl;
        return;
    }

    //create the master copy of the source string
    //string master = input + "$";
    _source = input + "$";
    _alphabet = alphabet;

    //create the root node
    _root = new SuffixNode(&_source);
    _root->_isRoot = true;
    _root->_isInternal = true;
    _root->_stringDepth = 0;
    _root->_parent = _root;
    _root->_suffixLink = _root;
    _numNodes += 1;
    _numInternal += 1;

    SuffixNode *lastLeaf = nullptr;

    int finalIndex = _source.length();

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
                hopSection.start = u->_edge.start + 1;
                hopSection.length = u->_edge.length - 1;
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
    if (n->_isInternal)
    {
        n->_id = idVal;
        idVal += 1;
        for (auto nodeptr : n->_children)
        {
            idVal = renumberInternalsHelper(nodeptr, idVal);
        }
    }
    return idVal;
}

void SuffixTree::BWT()
{
    bwtHelper(_root);
}

void SuffixTree::bwtHelper(SuffixNode *n)
{
    if (n->_id == 0 && n->_isLeaf)
    {
        cout << _source[_source.length() - 1] << endl;
    }
    else if (n->_isLeaf)
    {
        cout << _source[n->_id - 1] << endl;
    }
    else
    {
        for(int i = 0; i < n->_children.size(); i++){
            bwtHelper(n->_children[i]);
        }
    }
}

void SuffixTree::PrintTreeStatistics()
{
    cout << "+------------------------------+" << endl;
    cout << "Connor Wool's Suffix Tree Stats:" << endl;
    cout << "Nodes in tree:" << endl;
    cout << "\tInternal -- " << _numInternal << endl;
    cout << "\tLeaf ------ " << _numLeaves << endl;
    cout << "\tTotal ----- " << _numNodes << endl;
    int treeSizeBytes = _numNodes * sizeof(SuffixNode);
    cout << "Size of tree (bytes): " << treeSizeBytes << endl;
    cout << "Size of node (bytes): " << sizeof(SuffixNode) << endl;
    cout << "+------------------------------+" << endl;

}

void SuffixTree::dfsTraverse()
{
    cout << "Traverse" << endl;
    dfsTraverseHelper(_root);
    cout << "------------Finished" << endl;
}

int SuffixTree::dfsTraverseHelper(SuffixNode *n)
{
    cout << "(" << n->_id << ") " << "[" << n << "] ";
    cout << "(" << n->_edge.start << "," << n->_edge.length << ") --> ";
    for (auto childptr : n->_children)
    {
        cout << "[" << childptr << "](" << childptr->_edge.start << ",";
        cout << childptr->_edge.length << ") ";
    }
    cout << endl;
    for (auto childptr : n->_children)
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
        //if (n->_stringDepth >= _x)
        n->_start_leaf_index = _nextIndex;
        n->_end_leaf_index = _nextIndex;
        _nextIndex++;
        return;
    }
    if (n->_isInternal == true)
    {
        for (auto c : n->_children)
        {
            PrepareSTRecursive(c);
        }
        //if (n->_stringDepth >= _x)
        SuffixNode *uleft = n->_children[0];
        SuffixNode *uright = n->_children[n->_children.size() - 1];
        n->_start_leaf_index = uleft->_start_leaf_index;
        n->_end_leaf_index = uright->_end_leaf_index;
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

void SuffixTree::BruteFindLoc(string *read, vector<int> *positions){

    //variables to conduct search
    SuffixNode *current = nullptr;
    SuffixNode *next = nullptr;
    int readPtr = 0;

    //track deepest node visited
    int maxStringDepth = 0;
    SuffixNode *deepestNode = nullptr;

    for (int i = 0; i < read->length(); i++){
        current = _root;    //start each loop at the root
        readPtr = i;        //start the read pointer at next index each loop

        label_loop_start:

        //hop down nodes until mismatch found
        next = nullptr; //reset next to be null, so we can detect lack of child to continue along

        //select child of current node to continue along
        for (auto child : current->_children)
        {
            if (_source[child->_edge.start] == (*read)[readPtr])
            {
                next = child;
            }
        }

        //if the child is null, we've detected a mismatch
        //the current node should be checked for being the deepest node
        if(next == nullptr){
            if(current->_stringDepth > maxStringDepth){ //&& current->_stringDepth > _x){
                maxStringDepth = current->_stringDepth;
                deepestNode = current;
            }
            continue;
        }

        //otherwise, walk along it's edge label and search for a mismatch
        char *compare = &_source[next->_edge.start];
        bool matching = true;
        for (int cmpPtr = 0; (matching == true && cmpPtr < next->_edge.length); cmpPtr++)
        {
            //check if the current chars mismatch
            if (compare[cmpPtr] != (*read)[readPtr])
            {
                if(current->_stringDepth > maxStringDepth){ //} && next->_stringDepth > _x){
                    maxStringDepth = current->_stringDepth;
                    deepestNode = current;
                }
                matching = false;
            }
            else{
                readPtr++;
            }
        }

        if(matching){
            current = next;
            goto label_loop_start;
        }
    }

    if (deepestNode != nullptr)
    {
        //cout << "d:" << deepestNode->_stringDepth << " leaf?:" << deepestNode->_isLeaf << endl;
        for (int i = deepestNode->_start_leaf_index; i <= deepestNode->_end_leaf_index; i++)
        {
            positions->push_back(_A[i]);
        }
    }
}
