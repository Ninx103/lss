
#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <string>

#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_NO_DEPRECATED //recommended by boost for new code
#endif

#include "Filenumbers.h"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
using namespace std;
using namespace boost::filesystem;
using boost::regex_search;

typedef pair<list<BaseVersion>, list<Numberset>> filebaseset_t;

void lss( path & p ) 
{

	static map<string, filebaseset_t> filebasemap;

	
	boost::regex singlepattern("\\d");
	boost::regex multipattern("\\d+");
	for (directory_entry & filepath : directory_iterator(p)) {
		string filename = filepath.path().filename().string();
		string basename = boost::regex_replace(filename, singlepattern, "#");
		list<Numberset> filenumbers;
		boost::sregex_iterator iter(filename.begin(), filename.end(), multipattern);
		boost::sregex_iterator end;
		for (; iter != end; ++iter) {
			short pos = (*iter).position();
			string match = iter->str();			
			filenumbers.push_back(Numberset(pos, match));
		}

		auto B = filebasemap.find(basename);
		if (B == filebasemap.end()) {
			list<Numberset> files;
			BaseVersion B(filenumbers);
			list<BaseVersion> basefilenumbers = { B };
			filebaseset_t groups = { basefilenumbers , files };
			filebasemap.insert({ basename, groups });
		}
	}
}


int main( int argc, char ** argv )
{
	char * x = (argc < 2) ? "." : argv[1];
	path p(x);

	if (exists(p) && is_directory(p)) {
		lss(p);
	}
	else {
		printf("ERROR: Path \'%ls\' does not exists or is not a directory. Please verify the path.", p.c_str());
	}

	return 0;
}