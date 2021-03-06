#include "alignmentTable.h"


void AlignmentTable::printTable(){
    for(vector<tableCell> v1 : internal_table){
        for(tableCell t : v1){
            cout << "[" << t.insertionScore << ", " << t.matchScore << ", " << t.deletionScore << "] ";
        }
        cout << "\n";
    }
}

int AlignmentTable::scoreDel(int i, int j)
{
    if (i > 0)
    {
        //initialize max value to be a small int value
        int max = NEGATIVE_INFINITY;

        //get a pointer to the previous cell we want to reference
        tableCell *previous = &internal_table[i - 1][j];

        if (previous->deletionScore + this->scorer.g() > max)
        {
            max = previous->deletionScore + this->scorer.g();
        }
        if (previous->matchScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->matchScore + this->scorer.h() + this->scorer.g();
        }
        if (previous->insertionScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->insertionScore + this->scorer.h() + this->scorer.g();
        }

        //return the largest score we saw
        return max;
    }
    else
    {
        //if we're on the edge, return a small int value
        return NEGATIVE_INFINITY;
    }
}

int AlignmentTable::scoreIns(int i, int j)
{
    if (j > 0)
    {
        //initialize max value to be a small int value
        int max = NEGATIVE_INFINITY;

        //get a pointer to the previous cell we want to reference
        tableCell *previous = &internal_table[i][j - 1];

        if (previous->deletionScore + this->scorer.g() > max)
        {
            max = previous->deletionScore + this->scorer.h() + this->scorer.g();
        }
        if (previous->matchScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->matchScore + this->scorer.h() + this->scorer.g();
        }
        if (previous->insertionScore + this->scorer.h() + this->scorer.g() > max)
        {
            max = previous->insertionScore + this->scorer.g();
        }

        //return the largest score we saw
        return max;
    }
    else
    {
        //if we're on the edge, return a small int value
        return NEGATIVE_INFINITY;
    }
}

int AlignmentTable::scoreSub(int i, int j)
{

    if (i > 0 && j > 0)
    {
        //initialize max value to be a small int value
        int max = NEGATIVE_INFINITY;

        //get a pointer to the previous cell we want to reference
        tableCell *previous = &internal_table[i - 1][j - 1];

        if (previous->deletionScore > max)
        {
            max = previous->deletionScore;
        }
        if (previous->matchScore > max)
        {
            max = previous->matchScore;
        }
        if (previous->insertionScore > max)
        {
            max = previous->insertionScore;
        }

        //return the largest score we saw, with points for the match or mismatch
        if (s1[i] == s2[j])
        {
            return max + this->scorer.match();
        }
        else
        {
            return max + this->scorer.misMatch();
        }
    }

    else
    {
        //if we're on the edge, return a small int value
        return NEGATIVE_INFINITY;
    }
}

void AlignmentTable::scoreCell(int i, int j, int mode)
{
    tableCell *current = &internal_table[i][j];
    current->deletionScore = scoreDel(i, j);
    if(mode == 1 && current->deletionScore < 0){
        current->deletionScore = 0;
    }
    current->insertionScore = scoreIns(i, j);
    if(mode == 1 && current->insertionScore < 0){
        current->insertionScore = 0;
    }
    current->matchScore = scoreSub(i, j);
    if(mode == 1 && current->matchScore < 0){
        current->matchScore = 0;
    }
}

int AlignmentTable::getScore(int i, int j){
    tableCell *current = &internal_table[i][j];
    int max = NEGATIVE_INFINITY;
    if (current->matchScore > max){
        max = current->matchScore;
    }
    if (current->deletionScore > max){
        max = current->deletionScore;
    }
    if (current->insertionScore > max){
        max = current->insertionScore;
    }
    return max;
}

Direction AlignmentTable::getDirection(int i, int j){

    tableCell *current = &this->internal_table[i][j];

    int maxVal = getScore(i, j);
    if(current->matchScore == maxVal){
        return SUBSTITUTION;
    }
    if(current->deletionScore == maxVal){
        return DELETION;
    }
    if(current->insertionScore == maxVal){
        return INSERTION;
    }
}

//TODO:
//create empty table (vector of vectors)
//set string 1 and string 2 from vector of sequences
//run algorithm on first row and first column to prepare
void AlignmentTable::initTable()
{

    if (this->sequences.size() < 2)
    {
        cout << "not enough sequences... something went wrong... (#=" << this->sequences.size() << "\n";
        exit(1);
    }

    //set the sequences from the parser
    this->s1 = this->sequences[0].sequence;
    this->s2 = this->sequences[1].sequence;

    //set the size of the table from the sequence strings
    //leaves room for indel row/col in the first row and col
    this->ncols = 1 + s1.length();
    this->nrows = 1 + s2.length();

    for (int i = 0; i < nrows; i++)
    {
        vector<tableCell> temp;
        for (int j = 0; j < ncols; j++)
        {
            tableCell t;
            t.deletionScore = 0;
            t.insertionScore = 0;
            t.matchScore = 0;
            temp.push_back(t);
        }
        this->internal_table.push_back(temp);
    }

    cout << "in theory: rows:" << this->nrows << " cols:" << this->ncols << "\n";
    cout << "in practice: rows:" << this->internal_table.size();
    cout << "cols: " << this->internal_table[0].size() << "\n";
}

void AlignmentTable::fillTable(int mode)
{
    tableCell *current;

    if(mode == 0){
        cout << "Filling table for global alignment\n";
    }
    else{
        cout << "Filling table for local alignment\n";
        this->local_align_i = -1;
        this->local_align_j = -1;
        this->local_align_max = NEGATIVE_INFINITY;
    }

    if (mode == 0)   //prepare for global alignment
    {
        current = &this->internal_table[0][0];
        current->deletionScore = NEGATIVE_INFINITY;
        current->insertionScore = NEGATIVE_INFINITY;
        current->matchScore = 0;
        for (int i = 1; i < this->internal_table.size(); i++){
            current = &this->internal_table[i][0];
            current->matchScore = NEGATIVE_INFINITY;
            current->insertionScore = NEGATIVE_INFINITY;
            current->deletionScore = this->scorer.h() + (i * this->scorer.g());
        }
        for (int j = 1; j < this->internal_table[0].size(); j++){
            current = &this->internal_table[0][j];
            current->matchScore = NEGATIVE_INFINITY;
            current->deletionScore = NEGATIVE_INFINITY;
            current->insertionScore = this->scorer.h() + (j * this->scorer.g());
        }
    }

    else if(mode == 1){
        current = &this->internal_table[0][0];
        current->deletionScore = NEGATIVE_INFINITY;
        current->insertionScore = NEGATIVE_INFINITY;
        current->matchScore = 0;
        for (int i = 1; i < this->internal_table.size(); i++){
            current = &this->internal_table[i][0];
            current->matchScore = NEGATIVE_INFINITY;
            current->insertionScore = NEGATIVE_INFINITY;
            current->deletionScore = 0;
        }
        for (int j = 1; j < this->internal_table[0].size(); j++){
            current = &this->internal_table[0][j];
            current->matchScore = NEGATIVE_INFINITY;
            current->deletionScore = NEGATIVE_INFINITY;
            current->insertionScore = 0;
        }
    }

    this->printTable();
}

//global initializes the edges using the scoring method
void AlignmentTable::globalAlign()
{
    cout << "Running global alignment\n";
    for (int i = 1; i < this->internal_table.size(); i++)
    {
        for (int j = 1; j < this->internal_table[0].size(); j++){
            scoreCell(i, j, 0);
        }
    }

    this->printTable();
}

//local initialzes both local and global edges to 0
void AlignmentTable::localAlign()
{
    cout << "Running local alignment\n";
    for (int i = 1; i < this->internal_table.size(); i++)
    {
        for (int j = 1; j < this->internal_table[0].size(); j++){
            scoreCell(i, j, 1);
            if(getScore(i,j) > this->local_align_max){
                this->local_align_max = getScore(i, j);
                this->local_align_i = i;
                this->local_align_j = j;
            }
        }
    }

    this->printTable();
}

void AlignmentTable::retrace(int mode){

    this->s1_sequenced = "";
    this->s2_sequenced = "";
    this->s3_sequenced = "";

    cout << "Running retrace function\n";
    cout << "s1_sequenced: `" << s1_sequenced << "`\n";

    int i, j;

    //initialize our starting point in the table
    //we use the last square for global alignment
    //we use the max square for local alignment
    if(mode == 0){
        i = this->internal_table.size() -1 ;
        j = this->internal_table[0].size() -1 ;
    }
    else if(mode == 1){
        i = this->local_align_i;
        j = this->local_align_j;
    }

    cout << "Retrace values: i=" << i << " j=" << j << "\n";

    while(i >= 0 && j >= 0){
        Direction d = this->getDirection(i, j);
        switch(d){
            case DELETION:
                cout << "DELETION\n";
                this->s1_sequenced = "-" + s1_sequenced;//.insert(0, "-");
                this->s2_sequenced = s2[i] + s2_sequenced;//.insert(0, "" + this->s2[i]);
                this->s3_sequenced = " " + s3_sequenced;//.insert(0, " ");
                j--;
                break;
            case INSERTION:
                cout << "INSERTION\n";
                this->s1_sequenced = s1[j] + s1_sequenced;//.insert(0, "" + this->s1[j]);
                this->s2_sequenced = "-" + s2_sequenced;//.insert(0, "-");
                this->s3_sequenced = " " + s3_sequenced;//.insert(0, " ");
                i--;
                break;
            case SUBSTITUTION:
                cout << "SUBSTITUTION\n";
                this->s1_sequenced = s1[j] + s1_sequenced;//.insert(0, "" + this->s1[j]);
                this->s2_sequenced = s2[i] + s2_sequenced;//.insert(0, "" + this->s2[i]);
                if(this->s1[j] == this->s2[i]){
                    this->s3_sequenced = "|" + s3_sequenced;//.insert(0, "|");
                }
                else{
                    this->s3_sequenced = " " + s3_sequenced;//.insert(0, " ");
                }
                i--;
                j--;
                break;
            default:
                cout << "DEFAULT CASE, OH NO!\n";
            }
    }

    cout << "Printing final alignments\n";
    cout << "Printing only s1, starting next line:" << endl;
    //cout << "`" << this->s1_sequenced << "`\n";
    cout << s1_sequenced.substr(0,60) << endl;
    cout << s3_sequenced.substr(0,60) << endl;
    cout << s2_sequenced.substr(0,60) << endl;
}