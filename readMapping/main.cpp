

#include "suffixTree.h"
#include "fastaParser.h"
#include "alphabetParser.h"
#include "alignmentTable.h"

#include <string>
#include <vector>

using namespace std;

string referenceGenomeFastaFile;
string readFastaFile;
string inputAlphabetFile;

int main(int argc, char *argv[]){

    //a.out <fasta file g> <fasta file of reads> <input alphabet>
    if(argc == 4){
        referenceGenomeFastaFile = argv[1];
        readFastaFile = argv[2];
        inputAlphabetFile = argv[3];
    }
    else{
        cout << "usage: map <reference genome> <reads> <alphabet>" << endl;
        exit(1);
    }

    cout << "Reference file: " << referenceGenomeFastaFile <<endl;
    cout << "Read file: " << readFastaFile  << endl;
    cout << "Alphabet file: " << inputAlphabetFile << endl;

    FastaParser fpReference;
    FastaParser fpReads;
    AlphabetParser apAlpha;

    fpReference.setInputFile(referenceGenomeFastaFile);
    fpReference.readFile();
    vector<GeneSequence> refSeqVec = fpReference.getSequences();

    cout << "got " << refSeqVec.size() << " strings from refernece" << endl;

    fpReads.setInputFile(readFastaFile);
    fpReads.readFile();
    vector<GeneSequence> readSeqVec = fpReads.getSequences();
    cout << "got " << readSeqVec.size() << " strings from reads" << endl;

    apAlpha.setInputFile(inputAlphabetFile);
    apAlpha.readFile();
    string alphaString = apAlpha.getAlphabet();
    cout << "alphabet " << alphaString << endl;

}