//Unit testing for the classes in this project

#include "fastaParser.hpp"
#include <iostream>
#include "fastaParser.hpp"
#include "geneSequence.hpp"

using namespace std;

int testFastaParser(){
    FastaParser f = FastaParser();
    f.setInputFile("trivial.fasta");
    f.readFile();
    vector<GeneSequence> results;
    results = f.getSequences();

    cout << "results vector is of size: " << results.size() << "\n";

    for(GeneSequence g : results){
        cout << "Sequence Name: " << g.name << "\n";
        cout << "Sequence Value: " << g.sequence << "\n";
    }
    if(results[0].name.compare("sequence1") != 0){
        return 1;
    }
    if(results[1].name.compare("sequence2") != 0){
        return 2;
    }
    if(results[0].sequence.compare("ATGA") != 0){
        return 3;
    }
    if(results[1].sequence.compare("CTGT") != 0){
        return 4;
    }
    return 0;
}

int main(){

    int fastaTest;
    if (fastaTest = testFastaParser())
    {
        //if non-zero exit value, test failed
        cout << "FastaParser test failed: failure code: " << fastaTest << "\n";
    }
    else{
        //if zero exit value, test success
        cout << "FastaParser test success: success code: " << fastaTest << "\n";
    }
}

