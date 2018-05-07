#include "alignmentTable.h"

void AlignmentTable::localAlign(string *s1, string *s2, int *matches, int *alignLen){
    setSequences(s1, s2);
    initTable();
    fillTableLocal();
    localAlign();
    *matches = retraceLocal();
    *alignLen = _s1Seq.length();
}

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
    //set sequence strings
    _s1 = s1;
    _s2 = s2;
    //set size of table, leaving room for empty first row/col
    _numRows = (*_s2).length() + 1;
    _numCols = (*_s1).length() + 1;
    initTable();
}

void AlignmentTable::initTable()
{
    _cellTable.clear();
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
    //cout << "created table: ";
    //cout << _cellTable.size() << " rows, ";
    //cout << _cellTable[0].size() << " cols" << endl;
}

void AlignmentTable::fillTableGlobal()
{
    tableCell *current;

    //set first cell to zero
    current = &_cellTable[0][0];
    current->sub = 0;
    current->del = 0;
    current->ins = 0;

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
        current->sub = NEGATIVE_INF;
        current->del = NEGATIVE_INF;
    }

    for (int x = 1; x < _cellTable[0].size(); x++)
    {
        current = &_cellTable[0][x];
        current->del = 0;
        current->ins = NEGATIVE_INF;
        current->sub = NEGATIVE_INF;
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
        //tableCell *current = &_cellTable[x][y];
        current->sub = scoreSub(x, y);
        current->ins = scoreIns(x, y);
        current->del = scoreDel(x, y);
    }

    if (mode == 1) //local alignment scoring, don't allow negatives
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
    if (compare->del + _score_h + _score_g > max)
    {
        max = compare->del + _score_h + _score_g;
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
    //retraceLocal();
}

void AlignmentTable::retraceGlobal(){
    int y = _cellTable.size() -1 ;
    int x = _cellTable[0].size() - 1;

    tableCell *current = &_cellTable[y][x];

    _s1Seq = "";
    _s2Seq = "";
    _middleString = "";

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
        }

        else if(delMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = "-" + _s2Seq;
            _middleString = " " + _middleString;
            x--;
        }

        else if(subMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = (*_s2)[y-1] + _s2Seq;
            if((*_s1)[x-1] == (*_s2)[y-1]){
                _middleString = "|" + _middleString;
            }
            else{
                _middleString = " " + _middleString;
            }
            x--;
            y--;
        }

        current = &_cellTable[y][x];
    }

    while(x > 0){
        _s1Seq = (*_s1)[x-1] + _s1Seq;
        _s2Seq = "-" + _s2Seq;
        _middleString = " " + _middleString;
        x--;
    }

    while(y > 0){
        _s1Seq = "-" + _s1Seq;
        _s2Seq = (*_s2)[y-1] + _s2Seq;
        _middleString = " " + _middleString;
        y--;
    }
}

int AlignmentTable::retraceLocal(){

    int numMatches = 0;
    int xmax = -1;
    int ymax = -1;
    int valMax = NEGATIVE_INF;

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

    //cout << "xmax " << xmax << " ymax " << ymax << endl;

    _s1Seq = "";
    _s2Seq = "";
    _middleString = "";
    int x = xmax;
    int y = ymax;
    tableCell *current = &_cellTable[y][x];

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
        }

        else if(delMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = "-" + _s2Seq;
            _middleString = " " + _middleString;
            x--;
        }

        else if(subMax == max){
            _s1Seq = (*_s1)[x-1] + _s1Seq;
            _s2Seq = (*_s2)[y-1] + _s2Seq;
            if((*_s1)[x-1] == (*_s2)[y-1]){
                _middleString = "|" + _middleString;
                numMatches++;
            }
            else{
                _middleString = " " + _middleString;
            }
            x--;
            y--;
        }

        current = &_cellTable[y][x];
    }
    return numMatches;
}

vector<string> AlignmentTable::getAlignments()
{
    vector<string> results;
    results.push_back(_s1Seq);
    results.push_back(_s2Seq);
    return results;
}

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
    for (int i = 0; i < _s1Seq.length(); i += 60){
        string s1sub = _s1Seq.substr(i, 60);
        string s2sub = _s2Seq.substr(i, 60);
        string midsub = _middleString.substr(i, 60);
        cout << s1sub << endl;
        cout << midsub << endl;
        cout << s2sub << endl;
    }
}