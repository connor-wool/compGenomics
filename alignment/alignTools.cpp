/* -Connor Wool- -Spring 2018-
a library file containing tools that are useful in 
aligning two DNA sequences. Uses the Affine Align
method.

Designed to support both Global and Local alignments.

*/

#include <vector>
#include <string>

using namespace std;

typedef struct cell{
    int score;
    //other things
    int subScore;
    int delScore;
    int insScore;
}CELL;

class CellTable
{
    private:
    string s1;
    string s2;
    vector<vector<cell>> internal_table;

    public:
    void setupTable();

    void setStrings(string new_s1, string new_s2){
        this->s1 = new_s1;
        this->s2 = new_s2;
    }
};

void CellTable::setupTable(){
    //leave room for 0 in first row/col
    int nrows = 1 + s1.length();
    int ncols = 1 + s2.length();

    for(int i = 0; i < nrows; i++){
        vector<cell> temp;
        internal_table[i] = temp;
    }
}

class CellScorer
{
    private:
        //store the scores in the scoring object
        int matchScore;
        int misMatchScore;
        int h;
        int g;

    public:
        void setH(int new_h){
            this->h = new_h;
        }
        void setG(int new_g){
            this->g = new_g;
        }
        void setMatchScore(int new_match_score){
            this->matchScore = new_match_score;
        }
        void setMisMatchScore(int new_mis_match_score){
            this->misMatchScore = new_mis_match_score;
        }
        
};

int scoreCell(int row, int col){

}
