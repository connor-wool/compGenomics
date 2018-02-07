

#ifndef FASTAPARSER_H
#define FASTAPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "geneSequence.hpp"

using namespace std;

class FastaParser{
    private:
        vector<GeneSequence> sequences;
        string filename;
        int readFile();

    public:
        void setInputFile(string inputFile){
            this->filename = inputFile;
            readFile();
        }
};

int FastaParser::readFile(){

    if(this->filename.empty() || this->filename == ""){
        cout << "Error parsing FASTA file, no filename provided.";
        return -1;
    }

    string line;
    ifstream datafile;
    GeneSequence *currentSequence;
    datafile.open(this->filename);

    if(datafile.is_open()){
        while(getline(datafile,line)){
            
            //This case is true if we're starting a new sequence
            if(line[0] == '>'){

                //create a new sequence, get a reference, and add to the vector of sequences
                GeneSequence newSequence = GeneSequence();
                currentSequence = &newSequence;
                this->sequences.push_back(newSequence);

                stringstream s = stringstream(line);
                string sequenceID;
                s >> sequenceID;
                sequenceID.erase(0, 1); //remove the '>' character
                cout << sequenceID;
                currentSequence->name = sequenceID; //add this name to the GeneSequence object
            }

            //add the whole line to the current sequence
            else{
                currentSequence->sequence.append(line);
            }
        }
    }
    
}

#endif
