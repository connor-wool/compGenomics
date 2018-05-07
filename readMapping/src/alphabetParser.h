#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class AlphabetParser
{
  private:
    string alphabet;
    string filename;

  public:
    AlphabetParser();
    void setInputFile(string filename);
    void readFile();
    string getAlphabet();
};

AlphabetParser::AlphabetParser(){
    //empty constructor
}

void AlphabetParser::setInputFile(string filename){
    this->filename = filename;
}

void AlphabetParser::readFile(){
    if(this->filename.empty()){
        cout << "Error parsing alphabet file, no filename provided" << endl;
        exit(1);
    }

    string line;
    ifstream inputStream;
    inputStream.open(this->filename, std::ifstream::in);

    if(inputStream.is_open()){
        getline(inputStream, line);
        for (char c : line){
            if(c != ' '){
                this->alphabet = this->alphabet + c;
            }
        }
    }
}

string AlphabetParser::getAlphabet(){
    return this->alphabet;
}