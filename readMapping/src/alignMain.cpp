/* alignmentDriver.cpp
Demo code to run the alignment table project

$ a.out <input file with s1, s2> <0:global, 1:local> <config file>
*/

#include <iostream>
#include <string>

#include "alignmentCell.h"
#include "alignmentTable.h"
#include "fastaParser.h"
#include "geneSequence.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 3){
        cout << "Invalid number of parameters" << endl;
        cout << "Usage: align <inputFile> <0:g, 1,l> [config file]" << endl;
        exit(1);
    }

    string inputFile = argv[1];
    int alignType = atoi(argv[2]);
    string paramsFile = "parameters.config";
    if(argc == 4){
        paramsFile = argv[3];
    }

    FastaParser fp;
    fp.setInputFile(inputFile);
    vector<GeneSequence> gsv = fp.getSequences();
    //cout << gsv.size() << endl;

    AlignmentTable at;
    at.setSequences(&gsv[0], &gsv[1]);
    at.setScores(1,-2,-5,-2);
    if(alignType == 0){
        at.globalAlign();
    }
    else if(alignType == 1){
        at.localAlign();
    }
    //at.printTable();
    at.printSequences();
}