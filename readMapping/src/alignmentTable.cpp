#include "alignmentTable.h"

AlignmentTable::AlignmentTable(){
    reset();
}

void AlignmentTable::reset(){
    _s1 = nullptr;
    _s2 = nullptr;
    _hasGeneNames = false;
    _numRows = 0;
    _numCols = 0;
    _s1Seq = "";
    _s2Seq = "";
    _middleString = "";
    _cellTable.clear();
    _numMatches = 0;
    _numGaps = 0;
    _numMismatches = 0;
    _numOpeningGaps = 0;
    _alignmentScore = 0;
}

void AlignmentTable::setSequences(GeneSequence *gene1, GeneSequence *gene2){
    reset();
    _hasGeneNames = true;
    g1 = *gene1;
    g2 = *gene2;
    _s1 = &g1.sequence;
    _s2 = &g2.sequence;
    _numRows = (*_s2).length() + 1;
    _numCols = (*_s1).length() + 1;
    initTable();
}

/*
void AlignmentTable::localAlign(string *s1, string *s2, int *matches, int *alignLen){
    reset();
    setSequences(s1, s2);
    initTable();
    fillTableLocal();
    localAlign();
    *matches = retraceLocal();
    *alignLen = _s1Seq.length();
}
*/

int AlignmentTable::selectMax(int a, int b, int c)
{
    int max = a;
    if (b > max)
    {
        max = b;
    }
    if (c > max)
    {
        max = c;
    }
    return max;
}

void AlignmentTable::setScores(int match, int mismatch, int h, int g)
{
    _score_match = match;
    _score_mismatch = mismatch;
    _score_h = h;
    _score_g = g;
}

void AlignmentTable::setSequences(string *s1, string *s2)
{
    reset();
    _s1 = s1;
    _s2 = s2;
    _numRows = (*_s2).length() + 1;
    _numCols = (*_s1).length() + 1;
    initTable();
}

void AlignmentTable::initTable()
{
    for (int y = 0; y < _numRows; y++)
    {
        vector<tableCell> thisRow;
        for (int x = 0; x < _numCols; x++)
        {
            tableCell cell;
            cell.sub = 0;
            cell.del = 0;
            cell.ins = 0;
            thisRow.push_back(cell);
        }
        _cellTable.push_back(thisRow);
    }
}

void AlignmentTable::fillTableGlobal()
{
    tableCell *current;
    
    //set first cell to zero
    current = &_cellTable[0][0];
    current->sub = 0;
    current->del = NEGATIVE_INF;
    current->ins = NEGATIVE_INF;

    for (int y = 1; y < _cellTable.size(); y++)
    {
        current = &_cellTable[y][0];
        current->ins = _score_h + (y * _score_g);
        current->sub = NEGATIVE_INF;
        current->del = NEGATIVE_INF;
    }

    for (int x = 1; x < _cellTable[0].size(); x++)
    {
        current = &_cellTable[0][x];
        current->del = _score_h + (x * _score_g);
        current->ins = NEGATIVE_INF;
        current->sub = NEGATIVE_INF;
    }
}

void AlignmentTable::fillTableLocal(){
    tableCell *current;

    //set first cell to zero
    current = &_cellTable[0][0];
    current->sub = 0;
    current->del = 0;
    current->ins = 0;

    for (int y = 1; y < _cellTable.size(); y++)
    {
        current = &_cellTable[y][0];
        current->ins = 0;
        current->sub = 0;
        current->del = 0;
    }

    for (int x = 1; x < _cellTable[0].size(); x++)
    {
        current = &_cellTable[0][x];
        current->del = 0;
        current->ins = 0;
        current->sub = 0;
    }
}

void AlignmentTable::scoreCell(int x, int y, int mode)
{
    if (x == 0 || y == 0)
    {
        cout << "scoring shouldn't occur in edge cells" << endl;
        return;
    }

    tableCell *current = &_cellTable[y][x];

    if (mode == 0) //global alignment scoring, allow negatives
    {
        current->sub = scoreSub(x, y);
        current->ins = scoreIns(x, y);
        current->del = scoreDel(x, y);
    }

    else if (mode == 1) //local alignment scoring, don't allow negatives
    {
        current->sub = scoreSub(x,y);
        if(current->sub < 0){
            current->sub = 0;
        }
        current->ins = scoreIns(x,y);
        if(current->ins < 0){
            current->ins = 0;
        }
        current->del = scoreDel(x,y);
        if(current->del < 0){
            current->del = 0;
        }
    }
}

//side
int AlignmentTable::scoreDel(int x, int y)
{
    tableCell *current = &_cellTable[y][x];
    tableCell *compare = &_cellTable[y][x - 1];

    int max = NEGATIVE_INF;

    if (compare->del + _score_g > max)
    {
        max = compare->del + _score_g;
    }
    if (compare->sub + _score_h + _score_g > max)
    {
        max = compare->sub + _score_h + _score_g;
    }
    if (compare->ins + _score_h + _score_g > max)
    {
        max = compare->ins + _score_h + _score_g;
    }
    return max;
}

//top
int AlignmentTable::scoreIns(int x, int y)
{
    tableCell *current = &_cellTable[y][x];
    tableCell *compare = &_cellTable[y - 1][x];

    int max = NEGATIVE_INF;
    if (compare->ins + _score_g > max)
    {
        max = compare->ins + _score_g;
    }
    if (compare->del + _score_h + _score_g > max)
    {
        max = compare->del + _score_h + _score_g;
    }
    if (compare->sub + _score_h + _score_g > max)
    {
        max = compare->sub + _score_h + _score_g;
    }
    return max;
}

//diagonal
int AlignmentTable::scoreSub(int x, int y)
{
    tableCell *current = &_cellTable[y][x];
    tableCell *compare = &_cellTable[y - 1][x - 1];

    int max = selectMax(compare->sub, compare->del, compare->ins);

    if ((*_s1)[x] == (*_s2)[y])
    {
        return max + _score_match;
    }
    else
    {
        return max + _score_mismatch;
    }
}

void AlignmentTable::globalAlign()
{
    fillTableGlobal();
    for (int y = 1; y < _cellTable.size(); y++)
    {
        for (int x = 1; x < _cellTable[0].size(); x++)
        {
            scoreCell(x, y, 0);
        }
    }
    retraceGlobal();
}

void AlignmentTable::localAlign()
{
    fillTableLocal();
    for(int y = 1; y < _cellTable.size(); y++){
        for (int x = 1; x < _cellTable[0].size(); x++){
            scoreCell(x,y,1);
        }
    }
    retraceLocal();
}

void AlignmentTable::retraceGlobal(){
    int y = _cellTable.size() -1 ;
    int x = _cellTable[0].size() - 1;

    tableCell *current = &_cellTable[y][x];

    _alignmentScore = selectMax(current->ins, current->del, current->sub);

    _s1Seq = "";
    _s2Seq = "";
    _middleString = "";

    bool previousWasGap = false;

    while( x > 0 && y > 0){

        tableCell *ins = &_cellTable[y-1][x];
        tableCell *del = &_cellTable[y][x-1];
        tableCell *sub = &_cellTable[y-1][x-1];

        int insMax = selectMax(ins->ins, ins->del, ins->sub);
        int delMax = selectMax(del->ins, del->del, del->sub);
        int subMax = selectMax(sub->ins, sub->del, sub->sub);

        int max = selectMax(insMax, delMax, subMax);

        if(insMax == max){
            _s1Seq = "-" + _s1Seq;
            _s2Seq = (*_s2)[y-1] + _s2Seq;
            _middleString = " " + _middleString;
            y--;
            _numGaps++;
            if(previousWasGap == false){
                _numOpeningGaps++;
            }
            previousWasGap = true;
        }

        

        else if(delMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = "-" + _s2Seq;
            _middleString = " " + _middleString;
            x--;
            _numGaps++;
            if (previousWasGap == false)
            {
                _numOpeningGaps++;
            }
            previousWasGap = true;
        }

        else if(subMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = (*_s2)[y-1] + _s2Seq;
            if((*_s1)[x-1] == (*_s2)[y-1]){
                _middleString = "|" + _middleString;
                _numMatches++;
            }
            else{
                _middleString = " " + _middleString;
                _numMismatches++;
            }
            x--;
            y--;
            previousWasGap = false;
        }

        current = &_cellTable[y][x];
    }

    while(x > 0){
        _s1Seq = (*_s1)[x-1] + _s1Seq;
        _s2Seq = "-" + _s2Seq;
        _middleString = " " + _middleString;
        x--;
        _numGaps++;
        if(previousWasGap == false){
            _numOpeningGaps++;
        }
        previousWasGap = true;
    }

    while(y > 0){
        _s1Seq = "-" + _s1Seq;
        _s2Seq = (*_s2)[y-1] + _s2Seq;
        _middleString = " " + _middleString;
        y--;
        _numGaps++;
        if (previousWasGap == false)
        {
            _numOpeningGaps++;
        }
        previousWasGap = true;
    }
}

int AlignmentTable::retraceLocal(){

    //use these to find max score in table
    int xmax = -1;
    int ymax = -1;
    int valMax = NEGATIVE_INF;

    bool previousWasGap = false;

    for(int y = 0; y < _cellTable.size(); y++){
        for(int x = 0; x < _cellTable[0].size(); x++){
            tableCell *c = &_cellTable[y][x];
            if(selectMax(c->ins, c->del, c->sub) > valMax){
                valMax = selectMax(c->ins, c->del, c->sub);
                xmax = x;
                ymax = y;
            }
        }
    }

    _s1Seq = "";
    _s2Seq = "";
    _middleString = "";

    int x = xmax;
    int y = ymax;
    tableCell *current = &_cellTable[y][x];

    _alignmentScore = selectMax(current->ins, current->del, current->sub);

    while(selectMax(current->ins, current->del, current->sub) > 0){

        tableCell *ins = &_cellTable[y-1][x];
        tableCell *del = &_cellTable[y][x-1];
        tableCell *sub = &_cellTable[y-1][x-1];

        int insMax = selectMax(ins->ins, ins->del, ins->sub);
        int delMax = selectMax(del->ins, del->del, del->sub);
        int subMax = selectMax(sub->ins, sub->del, sub->sub);

        int max = selectMax(insMax, delMax, subMax);

        if(insMax == max){
            _s1Seq = "-" + _s1Seq;
            _s2Seq = (*_s2)[y-1] + _s2Seq;
            _middleString = " " + _middleString;
            y--;
            _numGaps++;
            if (previousWasGap == false)
            {
                _numOpeningGaps++;
            }
            previousWasGap = true;
        }

        else if(delMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = "-" + _s2Seq;
            _middleString = " " + _middleString;
            x--;
            _numGaps++;
            if (previousWasGap == false)
            {
                _numOpeningGaps++;
            }
            previousWasGap = true;
        }

        else if(subMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = (*_s2)[y-1] + _s2Seq;
            if((*_s1)[x-1] == (*_s2)[y-1]){
                _middleString = "|" + _middleString;
                _numMatches++;
            }
            else{
                _middleString = " " + _middleString;
                _numMismatches++;
            }
            x--;
            y--;
            previousWasGap = false;
        }

        current = &_cellTable[y][x];
    }
}

/*
vector<string> AlignmentTable::getAlignments()
{
    vector<string> results;
    results.push_back(_s1Seq);
    results.push_back(_s2Seq);
    return results;
}
*/

void AlignmentTable::printTable()
{
    for (int y = 0; y < _cellTable.size(); y++)
    {
        for (int x = 0; x < _cellTable[0].size(); x++)
        {
            tableCell *c = &_cellTable[y][x];
            int max = selectMax(c->ins, c->sub, c->del);
            cout << "[" << setw(4) << max << "] ";
        }
        cout << endl;
    }
}

void AlignmentTable::printSequences(){
    cout << "+------------------------+" << endl;
    cout << "Final Alignment Output" << endl;
    cout << "+------------------------+" << endl;
    cout << "Parameters: match = " << _score_match;
    cout << ", mismatch = " << _score_mismatch;
    cout << ", h = " << _score_h;
    cout << ", g = " << _score_g << endl << endl;

    if(_hasGeneNames){
        cout << "Sequence 1: `" << g1.name << "`, length = " << _s1->length() << endl;
        cout << "Sequence 2: `" << g2.name << "`, length = " << _s2->length() << endl << endl;
    }
    else{
        cout << "NOTICE: No sequence names avaliable" << endl;
        cout << "Sequence 1: `N/A`, length = " << _s1->length() << endl;
        cout << "Sequence 2: `N/A`, length = " << _s2->length() << endl << endl;
    }


    int s1Ptr = 0;
    int s2Ptr = 0;

    for (int i = 0; i < _s1Seq.length(); i += 60){

        string s1sub = _s1Seq.substr(i, 60);
        string s2sub = _s2Seq.substr(i, 60);
        string midsub = _middleString.substr(i, 60);

        s1Ptr++;
        s2Ptr++;

        //print the s1 string
        cout << "s1\t" << s1Ptr << "\t";
        cout << s1sub;
        for(auto c : s1sub){
            if(c != '-'){
                s1Ptr++;
            }
        }
        s1Ptr--;
        cout << "  " << s1Ptr << endl;

        cout << "\t\t" << midsub << endl;

        cout << "s2\t" << s2Ptr << "\t";
        cout << s2sub;
        for(auto c : s2sub){
            if(c != '-'){
                s2Ptr++;
            }
        }
        s2Ptr--;
        cout << "  " << s2Ptr << endl << endl;
    }

    cout << "Scores:" << endl;
    cout << "\tGlobal Optimal Score: " << _alignmentScore << endl;
    cout << "\tMatches: " << _numMatches << endl;
    cout << "\tMismatches: " << _numMismatches << endl;
    cout << "\tGaps: " << _numGaps << endl;
    cout << "\tOpening Gaps: " << _numOpeningGaps << endl;
    cout << "Identities:" << endl;
    cout << "\t " << _numMatches << "/" << _s1Seq.length() << "(" << ((double)_numMatches / _s1Seq.length()) * 100 << "%)" << endl;
    cout << "Gaps:" << endl;
    cout << "\t " << _numGaps << "/" << _s1Seq.length() << " (" << ((double)_numGaps / _s1Seq.length()) * 100 << "%)" << endl;
}

int AlignmentTable::getMatches(){
    return _numMatches;
}
    int AlignmentTable::getMismatches(){
        return _numMismatches;
    }
    int AlignmentTable::getScore(){
        return _alignmentScore;
    }
    int AlignmentTable::getGaps(){
        return _numGaps;
    }
    int AlignmentTable::getOpeningGaps(){
        return _numOpeningGaps;
    }
    int AlignmentTable::getAlignmentLength(){
        return _s1Seq.length();
    }