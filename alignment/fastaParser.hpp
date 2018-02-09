

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

    public:
        void setInputFile(string inputFile){
            this->filename = inputFile;
        }

        int readFile();

        vector<GeneSequence> getSequences();
};

vector<GeneSequence> FastaParser::getSequences(){
    
    cout << "copying sequences\n";
    cout << "internal sequence vector\n";
    for (GeneSequence g : this->sequences)
    {
        cout << g.name << " : " << g.sequence << "\n";
    }

    vector<GeneSequence> temp;
    temp = this->sequences;

    cout << "copied sequence vector\n";
    for (GeneSequence g : temp)
    {
        cout << g.name << " : " << g.sequence << "\n";
    }
    return temp;
}

int FastaParser::readFile(){

    if(this->filename.empty()){
        cout << "Error parsing FASTA file, no filename provided.";
        return -1;
    }

    string line;
    ifstream inputStream;
    GeneSequence *currentSequence = nullptr;
    inputStream.open(this->filename, std::ifstream::in);

    if (inputStream.is_open()){
        while(getline(inputStream,line)){

            if(line.empty()) { continue; }

            //char '>' indicates start of new sequence
            if(line[0] == '>'){

                GeneSequence newSequence = GeneSequence();
                currentSequence = &newSequence;
                this->sequences.push_back(newSequence);

                stringstream s = stringstream(line);
                string sequenceID;
                s >> sequenceID;
                sequenceID.erase(0, 1);         //removes the '>' character
                currentSequence->name = sequenceID;
                cout << "sequence name from GeneSequence object: `" << currentSequence->name << "`\n";
            }

            else{
                currentSequence->sequence.append(line);
            }
        }
    }
    
}

#endif
