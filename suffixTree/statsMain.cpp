#include "suffixNode.h"
#include "suffixTree.h"
#include "fastaParser.h"
#include "alphabetParser.h"
#include <chrono>

int main(int argc, char *argv[])
{
    //get the input file options from the commandline
    string inputFastaFile;
    string inputAlphabetFile;

    if (argc == 3)
    {
        inputFastaFile = argv[1];
        inputAlphabetFile = argv[2];
    }
    else{
        cout << "Invalid # options included" << endl;
        cout << "Usage: a.out <fastaFile> <alphabetFile>" << endl;
        exit(1);
    }

    //Create the suffix tree, fasta parser, and alphabet parser
    FastaParser fp;
    SuffixTree st;
    AlphabetParser ap;

    //read the fasta file
    fp.setInputFile(inputFastaFile);
    fp.readFile();
    vector<GeneSequence> sequences = fp.getSequences();
    string inputString = sequences[0].sequence;

    //read the alphabet file
    ap.setInputFile(inputAlphabetFile);
    ap.readFile();
    string alphabet = ap.getAlphabet();

    //time construction of the tree
    auto start = std::chrono::high_resolution_clock::now();
    st.Construct(inputString, alphabet);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = finish - start;

    cout << "Construction took " << totalTime.count() << " seconds";
    cout << " (AKA: " << (totalTime.count() * 1000000.0) << " microseconds)" << endl;
    st.PrintTreeStatistics();
    st.FindLongestMatchingRepeat();

    return 0;
}