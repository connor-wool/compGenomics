
#include "alignmentTable.h"

int main (int argc, char *argv[]){
    AlignmentTable table;
    table.setScores(1,-2,-5,-2);
    string s1 = "ACATGCTATGTATACCACGTATCCGATACCCCGT";
    string s2 = "ACATGCGACAGATCTCACTACTCCGATACCCCGT";
    table.setSequences(s1,s2);
    table.localAlign();
    table.printTable();
    table.printSequences();
}
