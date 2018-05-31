

#include "suffixTree.h"
#include "fastaParser.h"
#include "alphabetParser.h"
#include "alignmentTable.h"

#include <string>
#include <vector>
#include <chrono>
#include <fstream>

#define VERBOSE_OUTPUT 0

using namespace std;
using namespace std::chrono;

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

    FastaParser fpReference;
    FastaParser fpReads;
    AlphabetParser apAlpha;

    string outputFileName = "MappingResults_" + readFastaFile + ".txt";
    ofstream outputFile;
    outputFile.open(outputFileName);

    //read reference genome
    fpReference.setInputFile(referenceGenomeFastaFile);
    //fpReference.readFile();
    vector<GeneSequence> refSeqVec = fpReference.getSequences();
    cout << "Read in reference sequence of size [" << refSeqVec[0].sequence.length() << "]" << endl;

    //read reads fasta file
    fpReads.setInputFile(readFastaFile);
    //fpReads.readFile();
    vector<GeneSequence> readSeqVec = fpReads.getSequences();
    cout << "Got " << readSeqVec.size() << " reads from file" << endl;

    //read alphabet file
    apAlpha.setInputFile(inputAlphabetFile);
    apAlpha.readFile();
    string alphaString = apAlpha.getAlphabet();

    // (1) Construct Suffix Tree
    SuffixTree tree;
    auto constructionStart = high_resolution_clock::now();
    tree.Construct(refSeqVec[0].sequence, apAlpha.getAlphabet());
    auto constructionEnd = high_resolution_clock::now();
    auto durationMs = duration_cast<milliseconds>(constructionEnd - constructionStart);
    cout << "Construction complete: " << durationMs.count() << " milliseconds ";
    auto durationSec = duration_cast<seconds>(constructionEnd - constructionStart);
    cout << "(" << durationSec.count() << " seconds)" << endl;
    tree.PrintTreeStatistics();

    // (2) Prepare Suffix Tree
    cout << "Preparing Suffix Tree for mapping reads" << endl;
    auto prepareStart = chrono::high_resolution_clock::now();
    tree.PrepareST();
    auto prepareEnd = chrono::high_resolution_clock::now();
    durationMs = duration_cast<milliseconds>(prepareEnd - prepareStart);
    cout << "Prepare Suffix Tree complete: " << durationMs.count() << " milliseconds ";
    durationSec = duration_cast<seconds>(prepareEnd - prepareStart);
    cout << "(" << durationSec.count() << " seconds)" << endl;

    // (3) Map Reads
    cout << "Mapping reads to reference genome" << endl;
    AlignmentTable atable;
    atable.setScores(1, -2, -5, -1);

    int totalAlignments = 0;
    int totalHits = 0;
    
    auto mapStart = chrono::high_resolution_clock::now();
    vector<int> positions;
    for (GeneSequence g : readSeqVec)
    {
        double LengthCoverage = 0;
        int maxHitStart = -1;
        int maxHitEnd = -1;

        positions.clear();
        tree.BruteFindLoc(&g.sequence, &positions);
        //tree.QuickFindLoc(&g.sequence, &positions);

        for (int p : positions)
        {
            int lowerBound = p - g.sequence.length();
            if(lowerBound < 0){
                lowerBound = 0;
            }

            int subsequencelength = 3 * g.sequence.length();
            string referenceSubstring = refSeqVec[0].sequence.substr(lowerBound, subsequencelength);

            atable.setSequences(&g.sequence, &referenceSubstring);
            atable.localAlign();

            int matches = atable.getMatches();
            int length = atable.getAlignmentLength();
            totalAlignments++;

            double loc_percentIdentity = (double)matches / (double)length;
            double loc_lenCoverage = (double)length / (double)g.sequence.length();
            
            //cout << "\tmatches: " << matches << " length: " << length;
            //cout << " ident: " << loc_percentIdentity;
            //cout << " coverage: " << loc_lenCoverage << endl;

            if(loc_percentIdentity >= 0.9 && loc_lenCoverage >= .8){
                if(loc_lenCoverage > LengthCoverage){
                    LengthCoverage = loc_lenCoverage;
                    maxHitStart = p;
                    maxHitEnd = p + g.sequence.length();
                }
            }
        }
        if(LengthCoverage == 0){
            if(VERBOSE_OUTPUT){
                cout << g.name << " No hit found ";
                cout << "(" << positions.size() << " pos.)" << endl;
            }
            outputFile << g.name << " No hit found" << endl;
        }
        else{
            if(VERBOSE_OUTPUT){
                cout << g.name << "\t" << maxHitStart << "\t" << maxHitEnd;
                cout << "\t(" << positions.size() << " pos.)" << endl;
            }
            outputFile << g.name << " " << maxHitStart << " " << maxHitEnd << endl;
            totalHits++;
        }
    }
    auto mapEnd = chrono::high_resolution_clock::now();
    durationMs = duration_cast<milliseconds>(mapEnd - mapStart);
    cout << "Read Mapping complete: " << durationMs.count() << " milliseconds ";
    durationSec = duration_cast<seconds>(mapEnd - mapStart);
    cout << "(" << durationSec.count() << " seconds)" << endl;

    cout << "Performed " << totalAlignments << " alignments on " << readSeqVec.size() << " reads." << endl;
    cout << "Average: " << ((double)totalAlignments / (double)readSeqVec.size()) << " alignments per read" << endl;
    cout << "Hit Rate: " << ((double)totalHits / readSeqVec.size()) * 100 << "%" << endl;

    durationMs = duration_cast<milliseconds>(mapEnd - constructionStart);
    cout << "Program complete: " << durationMs.count() << " milliseconds ";
    durationSec = duration_cast<seconds>(mapEnd - constructionStart);
    cout << "(" << durationSec.count() << " seconds)" << endl;

    outputFile.close();
}