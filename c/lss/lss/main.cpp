
#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_NO_DEPRECATED //recommended by boost for new code
#endif

#include "lss.h"

using namespace std;


int main( int argc, char ** argv )
{
	char * p = (argc < 2) ? "." : argv[1];
	lss::lss(p);

	return 0;
}

/*

In this side of the code, I decided to have a little more fun with objectifying 
the logic from the python code. I didn't make too many major changes from that code to
this, except I created custom object types to better suit the logic. 


I started with the idea that I wanted to generate a mapping sequence of
file base names with numbers replaced with the '#' octothorpe character.

i.e. file01_0001.rgb -> file##_####.rgb

So as I loop through the entire list of files, I'm generating a base name string 
for later lookup. The string of numbers that are stripped from the name get placed into a 
custom struct NumbersSet object  along with the string translated into its int equivalent. 
As I collect each NumberSet from the filename string, I place the string position 
and NumberSet into a map (typdef Numbers_t). This will preserve the
padding along with storing the int value of the number for sequence grouping later.

VERSION:
file01_0001.rgb -> Bases [{4:('01', 1), 7:('0001', 1)}]
                -> Files [{4:('01', 1), 7:('0002', 1)}, {4:('01', 1), 7:('0003', 1)}]

This begins to generate a list of "base versions". The idea here
is to figure out unique/non-sequencing files while looping through all the files.
As a NumberSet is created, it is compared to the list of base to determine if the 
numbers are incremented or a completely new base. 

If any of the NumberSet items are similar to any of the bases, I determine
that it *could* be an incremented file; I add 1 to the base count along with
adding 1 to its corresponding position that incremented in the poscount,
and push to the Files list.

If NONE of the bases increment then it is considered a new base, A base is generated
from the NumberSet and it is added to the group's Bases list. I then loop through all
the NumberSets already pushed into the Files list to account for NumberSets already pushed 
through that belong to the new base. I did it this way to reduce the number of files to loop
back over as new bases are registered.

file01_0001.rgb
file01_0002.rgb
Here's an example of a mapped filename with two files:
{'file#_#.rgb': {'groups': [{'base': {4:('01', 1), 7:('0001', 1)},
'count': 2,
'poscount': {7:2}}]
'files': [{4:('01', 1), 7:('0002', 2)}]

The last bit of logic comes in at the print for loop. I determine that files
with the largest file increment counts should go first since they are the
highest likely to be the correct file sequence in a directory. 

I loop over the bases and sort the BaseGroup Bases list by the highest count. 
For each base, I sort each of the positions by the highest position count, 
(and again by the "highest" right-most position as the tie-breaker).
As I loop through the Bases, I removing individual Files that increment at the
base's highest position count from the Files list and collect those numbers for my range.

My main goal here was to not build an algorithm that had O(n^2) notation. I also didnt want
to ping the filesystem numerous amounts of time as well to find best-guesses. I wanted
to loop through as little much as possible with most cases of directories.
Because the challenge indicated that the increments could be in any number position,
this can still cause the algorithm to have poor performance based on the contents
of the directory.

I suppose this is why we use file naming standards.

i.e. file001_[001-100].rgb, file002_[001-100].rgb, file003_[001-100].rgb, ... file100_[001-100].rgb
"""
*/