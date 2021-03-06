  ___ ___   _   ___  __  __ ___  _       _   
 | _ \ __| /_\ |   \|  \/  | __|| |___ _| |_ 
 |   / _| / _ \| |) | |\/| | _| |  _\ \ /  _|
 |_|_\___/_/ \_\___/|_|  |_|___(_)__/_\_\\__|

"README.TXT - A HELPFUL readme FILE saying how to compile and run the code"

There are two main binaries that can be generated by this project. They are `stats.out` and `bwt.out`. Each of these files is designed to create a specific output.

The `stats.out` binary creates a SuffixTree, and prints out some basic statistics (as per the project specification) about that tree. 

The `bwt.out` binary creates a SuffixTree, and prints out the BWT index of the tree, in a format that is desgined to match the examples provided with the project specification.

To generate the binaries, use the shell scripts included with the code. These are `buildStats.sh` and `buildBWT.sh` to build the stats and bwt binaries, respectively.

To run the binaries, use the appropriate command format:
$ stats.out <input_fasta_file> <input_alphabet_file>
$ bwt.out <input_fasta_file> <input_alphabet_file>