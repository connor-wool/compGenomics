#include "suffixTree.h"

SuffixTree::SuffixTree(){
    this->root = nullptr;
}

void SuffixTree::FindPath(SuffixNode *start, string s){
    //do the thing
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