#include "suffixNode.h"
#include "suffixTree.h"
#include "fastaParser.h"
#include "alphabetParser.h"

int main(int argc, char *argv[])
{
    //Create the suffix tree, fasta parser, and alphabet parser
    FastaParser fp;
    SuffixTree st;
    AlphabetParser ap;

    //read the fasta file
    fp.setInputFile("s1.fas");
    fp.readFile();
    vector<GeneSequence> sequences = fp.getSequences();
    string inputString = sequences[0].sequence;

    //read the alphabet file
    ap.setInputFile("English_alphabet.txt");
    ap.readFile();
    string alphabet = ap.getAlphabet();

    //construct the tree
    st.Construct(inputString, alphabet);
    st.dfsTraverse();

    return 0;
}