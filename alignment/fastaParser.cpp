#include "fastaParser.hpp"

//returns a deep copy of the internal GeneSequence vector
vector<GeneSequence> FastaParser::getSequences(){
    vector<GeneSequence> temp;
    temp = this->sequences;
    return temp;
}

int FastaParser::readFile(){

    cout << "Parsing FASTA file\n";

    if(this->filename.empty()){
        cout << "Error parsing FASTA file, no filename provided.";
        return -1;
    }

    string line;            //stores return data from each readline call
    ifstream inputStream;   //stream object handling input file
    int index = -1;         //used to track current GeneSequence object

    //opens the file for reading only
    inputStream.open(this->filename, std::ifstream::in);

    if(inputStream.is_open()){
        while(getline(inputStream,line)){

            if(line.empty()) { continue; }  //skip over empty lines

            /* char '>' indicates start of new sequence. We will
            add a new GeneSequence object to the vector, increment
            the index value, and set the name of the GeneSequence object. */
            if(line[0] == '>'){
                this->sequences.push_back(GeneSequence());
                index++;
                stringstream s = stringstream(line);
                string sequenceID;
                s >> sequenceID;
                sequenceID.erase(0, 1);     //removes the '>' character
                this->sequences[index].name = sequenceID;
            }

            /* If the line isn't empty, and doesn't start a new sequence,
            we can safely append the whole line to the current sequence. */
            else{
                this->sequences[index].sequence.append(line);
            }
        }
    }

    cout << "FASTA file parsed!\n";
    for(GeneSequence g : this->sequences){
        cout << "++++++++++\n";
        cout << "Name: `" << g.name << "`\n";
        cout << "Sequence:\n" << g.sequence << "\n";
        cout << "++++++++++\n";
    }
}