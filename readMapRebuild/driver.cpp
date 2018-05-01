#include "suffixNode.h"
#include "suffixTree.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    string s1 = "banana";
    string alpha = "abn";
    SuffixTree t;
    t.Construct(s1, alpha);
}