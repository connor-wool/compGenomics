/*
Local and Global Affine-Allignment functions
Written by Connor Wool
Cpts 471, WSU, Spring 2018
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "alignmentTable.hpp"
#include "affineScorer.hpp"

using namespace std;

void 

int main(int argc, char *argv[]){

    // ./a.out inputFileName <0:global 1:local> <optional:parametersFile>
    
    int shouldReadParamsFile = 0;

    if(argc < 3){
        cout << "Usage: filename <FastaSequenceFile> <0:global, 1:local> <optional:parametersFile>\n";
        cout << "Invalid Parameters entered. Exiting\n";
        exit(1);
    }
    
    string inputFileName = argv[1];
    int userChoice;
    stringstream(argv[2]) >> userChoice;
    if(userChoice > 1 || userChoice < 0){
        cout << "User option has invalid value: " << userChoice << "\n";
        cout << "value can either be 0 (global alignment) or 1 (local alignment)\n";
        cout << "Exiting...\n";
        exit(1);
    }

    cout << "Sequence File: " << inputFileName << "\n";
    cout << "global/local: " << userChoice << "\n";

    string paramFileName;
    if(argc == 4){
        paramFileName = argv[3];
        shouldReadParamsFile = 1;
        cout << "param file: " << paramFileName << "\n";
    }

    if(shouldReadParamsFile){

    }
    

}

