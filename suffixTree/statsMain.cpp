#include "suffixNode.h"
#include "suffixTree.h"
#include "fastaParser.h"
#include "alphabetParser.h"
#include <sys/time.h>
#include <string.h>

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

    //construct the tree

    struct timeval startConstruction, finishConstruction;
    memset(&startConstruction, 0, sizeof(startConstruction));
    memset(&finishConstruction, 0, sizeof(finishConstruction));

    gettimeofday(&startConstruction, 0);
    st.Construct(inputString, alphabet);
    gettimeofday(&finishConstruction, 0);

    int totalTime = finishConstruction.tv_usec - startConstruction.tv_usec;

    cout << "Construction took " << totalTime << " microseconds ";
    cout << " (AKA: " << ((double)totalTime / 1000000.0) << " seconds)" << endl;
    st.PrintTreeStatistics();

    return 0;
}