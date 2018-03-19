
#include <string>
#include "suffixNode.h"

class SuffixTree{
    private:
        SuffixNode *root;
        void FindPath(SuffixNode *start, String s);
        void NodeHops();
    public:
        SuffixTree();
        void Construct(String input, String alphabet);
        void DisplayChildren(SuffixNode *u);
}