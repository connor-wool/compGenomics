#include "suffixTree.h"
#include "fastaParser.h"
#include "alphabetParser.h"

using namespace std;

int main(int argc, char *argv[]){
    SuffixTree t;
    string inputFastaFile;
    string inputAlphabetFile;

    if(argc == 3){
        inputFastaFile = argv[1];
        inputAlphabetFile = argv[2];
    }
    else{
        cout << "Invalid # options included" << endl;
        cout << "Usage: a.out <fastaFile> <alphabetFile>" << endl;
        exit(1);
    }

    FastaParser fp;
    AlphabetParser ap;

    fp.setInputFile(inputFastaFile);
    fp.readFile();
    vector<GeneSequence> sequences = fp.getSequences();
    string inputString = sequences[0].sequence;

    ap.setInputFile(inputAlphabetFile);
    ap.readFile();
    string alphabet = ap.getAlphabet();

    t.Construct(inputString, alphabet);
    //t.BWT();
    t.PrintTreeStatistics();

    return 0;
}