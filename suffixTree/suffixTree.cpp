#include "suffixTree.h"

SuffixTree::SuffixTree(){
    this->root = nullptr;
}
/* finds the path starting at the specified node argument that spells out the
longest possible prefix of the specified string argument, and then insert
the next suffix */
void SuffixTree::FindPath(SuffixNode *start, string s, int suffixNumber){

    /* First, we search the children of the current node, looking for a pathlabel
    starting with the same character as the suffix we want to insert */
    SuffixNode *nextHop = nullptr;
    for (SuffixNode *child : start->getChildren())
    {
        char firstChildChar = child->getEdgeLabel().at(0);
        if(firstChildChar == s[0]){
            nextHop = child;
        }
    }

    /* base case: the starting node does not have a child that continues our
    insertion string. We create a new leaf node in the heap, and add a pointer
    to the new node in the children of the start node */
    if(nextHop == nullptr){
        SuffixNode *newChild = new SuffixNode();
        newChild->setAsLeaf();
        newChild->setEdgeLabel(s);
        newChild->setParent(start);
        newChild->setId(suffixNumber);
        start->addChild(newChild);
        return;
    }

    /* recursion case: at least a partial string match exists for the string
    we want to insert. Follow it down either until the string does not match,
    or until we get to another node. */
    else{
        string comparisonString = nextHop->getEdgeLabel();
        for (int i = 0; i < comparisonString.length(); i++){
            if(comparisonString[i] != s[i]){
                //break the string, add a node...
                string topHalfString = comparisonString.substr(0, i);
                string bottomHalfString = comparisonString.substr(i);
            }
        }
        FindPath(nextHop, s.substr(comparisonString.length()), suffixNumber);
        return;
    }
}

void SuffixTree::NodeHops(SuffixNode *start, string s){
    //do a thing? The thing? This thing?
}

void SuffixTree::Construct(string input, string alphabet){
    
    //verify string is in alphabet
    if(VerifyAlphabet(input, alphabet)){
        cout << "Input string and alphabet verified" << endl;
    }
    else{
        cout << "ERROR: input string not in alphabet" << endl;
        exit(1);
    }

    //add $ to end of input string
    string master = input + "$";

    //create a root node
    this->root = new SuffixNode(0);
    this->root->setAsRoot();
    this->root->setAsInternal();

    for (int loopControl = 0; loopControl < master.length(); loopControl++){
        string nextSuffix = master.substr(loopControl);
        cout << "Debug: nextSuffix is `" << nextSuffix << "`" << endl;

    }
}

bool SuffixTree::VerifyAlphabet(string input, string alphabet){
    for (char c : input){
        int pos = alphabet.find(c);
        if(pos < 0){
            return false;
        }
    }
    return true;
}