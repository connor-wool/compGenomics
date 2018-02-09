//Unit testing for the classes in this project

#include "fastaParser.hpp"
#include <iostream>
#include <vector>
#include "fastaParser.hpp"
#include "geneSequence.hpp"

using namespace std;

void printResults(int code, string name){
    if(code){
        cout << "TEST: " << name << " Failed, code: " << code << "\n";
    }
    else{
        cout << "TEST: " << name << " Passed, code: " << code << "\n";
    }
}

int testFastaParserFull(){
    FastaParser f = FastaParser();
    f.setInputFile("Opsin1_colorblindness_gene.fasta");
    f.readFile();
    vector<GeneSequence> results = f.getSequences();
    for(GeneSequence g : results){
        cout << "Name: " << g.name << "\n";
        cout << "Sequence: " << g.sequence << "\n";
        cout << "\n";
    }
    return 0;
}

void testFastaParserTrivial(){
    FastaParser f = FastaParser();
    f.setInputFile("trivial.fasta");
    f.readFile();
    vector<GeneSequence> results;
    results = f.getSequences();
    cout << "(FASTA): results vector size:\t" << results.size() << "\n";
    for(GeneSequence g : results){
        cout << "(FASTA): Sequence Name:\t\t`" << g.name << "`\n";
        cout << "(FASTA): Sequence Value:\t`" << g.sequence << "`\n";
    }
    if(results[0].name.compare("sequence1") != 0){
        printResults(1, "FastaParser");
    }
    if(results[1].name.compare("sequence2") != 0){
        printResults(2, "FastaParser");
    }
    if(results[0].sequence.compare("ATGA") != 0){
        printResults(3, "FastaParser");
    }
    if(results[1].sequence.compare("CTGT") != 0){
        printResults(4, "FastaParser");
    }
    printResults(0, "FastaParser");
}

int main(){
    cout << "TEST: Testing FastaParser\n";
    testFastaParserTrivial();
    //testFastaParserFull();
}
