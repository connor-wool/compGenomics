#include "affineScorer.hpp"

int AffineScorer::parseFile(string paramFileName){

    cout << "Parsing parameters file\n";

    if(paramFileName.empty()){
        cout << "Error parsing params file, no filename provided.";
        return -1;
    }

    ifstream paramFileStream;
    string line, key;
    int value;
    paramFileStream.open(paramFileName, ifstream::in);
    if(paramFileStream.is_open()){
        while(getline(paramFileStream, line)){
            if(line.empty()) {
                continue;
            }
            stringstream s = stringstream(line);
            s >> key;
            s >> value;
            if(key.compare("match") == 0){
                this->matchScore = value;
            }
            if(key.compare("mismatch") == 0){
                this->misMatchScore = value;
            }
            if(key.compare("h") == 0){
                this->hScore = value;
            }
            if(key.compare("g") == 0){
                this->gScore = value;
            }
        }
    }
    cout << "Finished parsing params file\n";
    cout << "match: " << this->matchScore << "\n";
    cout << "mismatch: " << this->misMatchScore << "\n";
    cout << "h: " << this->hScore << "\n";
    cout << "g: " << this->gScore << "\n";
}

int AffineScorer::scoreCell(int row, int col){
    return 1;
}