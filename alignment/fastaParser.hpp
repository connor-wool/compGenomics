/*
Connor Wool - Spring 2018
WSU; Cpts 471; Computational Genomics
Prof. Ananth;

This class defines an object that knows how to read and handle FASTA formatted files.

To use it, first the "setInputFile" function must be called to tell the object the name
of the file to read. Then, the "readFile" function is called. This causes the object
to process the given file, and store the sequences in an internal vector of "GeneSequence" objects.
Finally, the function "getSequences" returns a copy of the internal vector of sequences.

In the future, I'd like to have this class inherit from a parent class of "FileParser",
but that's outside the current scope of work.
*/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "geneSequence.h"

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