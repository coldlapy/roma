

Before running, there must be an input.txt file in the root directory, after running, an output.txt file is generated in the root directory.

Usage:

	cd src && make
	./roma


Assumptions: 

    Words in each line are separated by spaces, and multiple spaces are treated as one space.
    Problem statement after assignment and Credits statement.
    No empty lines.
    The format of the input line is fixed.


Design :

    1. Parse the data of each line to get the type of line, and the tokens array.
    2. Loop this array, do assignment, credits calculation and answer content generation according to the type of line.
    3. Output file


