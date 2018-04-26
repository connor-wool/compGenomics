/*
Local and Global Affine-Allignment functions
Written by Connor Wool
Cpts 471, WSU, Spring 2018
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "alignmentTable.h"
//#include "affineScorer.hpp"

using namespace std;

int main(int argc, char *argv[]){

    AlignmentTable table;       //where the magic happens
    int userChoice;             //stores choice of 0(global) 1(local)
    string sequenceFilename;    //path to sequence file
    string paramsFilename;      //path to parameters file

    if (argc < 3)
    {
        cout << "Usage: filename <FastaSequenceFile> <0:global, 1:local> <optional:parametersFile>\n";
        exit(1);
    }
    
    //read sequence file and choice from commandline
    sequenceFilename = argv[1];
    stringstream(argv[2]) >> userChoice;

    //error checking on user choice
    if(userChoice > 1 || userChoice < 0){
        cout << "User option has invalid value: " << userChoice << "\n";
        cout << "value can either be 0 (global alignment) or 1 (local alignment)\n";
        exit(1);
    }

    //set parameters file path appropriately
    if(argc > 3){
        paramsFilename = argv[3];
    }
    else{
        paramsFilename = "parameters.config";
    }

    cout << "===== INPUT VALUES =====\n";
    cout << "Sequence File:\t" << sequenceFilename << "\n";
    cout << "Global/Local:\t" << userChoice << "\n";
    cout << "Params file:\t" << paramsFilename << "\n";
    cout << "========================\n";

    table.initScorer(paramsFilename);
    table.initSequences(sequenceFilename);
    table.initTable();

    if(userChoice == 0){
        table.fillTable(0);
        table.globalAlign();
    }

    if(userChoice == 1){
        table.fillTable(1);
        table.localAlign();
    }

    table.retrace(0);
}
