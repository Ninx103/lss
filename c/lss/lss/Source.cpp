
#include <cstdio>
#include <iostream>
#include <numeric>

#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_NO_DEPRECATED //recommended by boost for new code
#endif

#include "Filenumbers.h"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
using namespace std;
using namespace boost::filesystem;
using boost::regex_search;
using boost::regex_replace;
using boost::regex;
using boost::sregex_iterator;

typedef pair<list<BaseNumberSet>, list<Numbers_t>> filebaseset_t;

string generateRangeStr( list<int> & filelist )
{
	string range = "";
	vector<int> diff, mylist;
	filelist.sort();
	copy(begin(filelist), end(filelist), back_inserter(diff));
	copy(begin(filelist), end(filelist), back_inserter(mylist));
	
	adjacent_difference(filelist.begin(), filelist.end(), diff.begin());

	vector<size_t> idx(filelist.size());
	iota (idx.begin(), idx.end(), 0);
	auto sep = [&diff](size_t i) { return diff[i] > 1; };
	for (auto p = idx.begin(); p != idx.end();)
	{
		range += to_string(mylist[*p]);
		p = find_if(++p, idx.end(), sep);
		range += "-" + to_string(mylist[*(p - 1)]) + " ";
	}
	return range;
}

bool compareCounts(BaseNumberSet & lhs, BaseNumberSet & rhs)
{
	return (lhs.getCount() > rhs.getCount());
}

void lss( path & p ) 
{
	static map<string, filebaseset_t> filebasemap;

	regex singlepattern("\\d");
	regex multipattern("\\d+");
	for (directory_entry & filepath : directory_iterator(p)) {
		string filename = filepath.path().filename().string();
		string basename = regex_replace(filename, singlepattern, "#");
		
		Numbers_t filenumbers;

		sregex_iterator iter(filename.begin(), filename.end(), multipattern);
		sregex_iterator end;
		for (; iter != end; ++iter) {
			const int pos = (*iter).position();
			filenumbers[pos] = {0, iter->str(), stoi(iter->str())};
		}

		if (filebasemap.find(basename) == filebasemap.end()) {
			list<BaseNumberSet> basefilenumbers = { BaseNumberSet(filenumbers) };
			filebaseset_t groups = { basefilenumbers , list<Numbers_t>()};
			filebasemap.insert({ basename, groups });
		}
		else {
			bool Found = false;
			for (auto & base : filebasemap[basename].first) {
				Found = (base.IncrementBaseSet(filenumbers)) ? true : false;
			}
			if (Found) {
				filebasemap[basename].second.push_back(filenumbers);
			}
			else {
				BaseNumberSet B(filenumbers);
				for (auto file : filebasemap[basename].second) {
					B.IncrementBaseSet(file);
				}
				filebasemap[basename].first.push_back(B);
			}
		}
	}

	for (auto filename : filebasemap) {
		filename.second.first.sort(compareCounts);
		for (auto basefilename : filename.second.first) {
			string basename = basefilename.FileStr(filename.first);
			string sequencerange = "";
			int pos = basefilename.getHighestPositionCount();
			list<int> filelist = { basefilename[pos].intnum };
			auto file = filename.second.second.begin();
			while( file != filename.second.second.end() ) {
				if (basefilename.isIncrementOf(*file, pos)) {
					filelist.push_back((*file)[pos].intnum);
					file = filename.second.second.erase(file);
				}
				else {
					++file;
				}
			}

			if (filelist.size() > 1) {
				string str = basefilename[pos].strnum;
				string padding = (str.size() > 1) ? "%0" + to_string(str.size()) + "d" : "%d";
				basename.replace(pos, str.size(), padding);
				sequencerange = generateRangeStr(filelist);
			}

			basename.insert(basename.size(), 25-basename.size(), ' ');
			cout << filelist.size() << " " << basename << " " << sequencerange << endl;
		}
	}
}


int main( int argc, char ** argv )
{
	char * x = (argc < 2) ? "." : argv[1];
	path p(x);

	//path p("..\\..\\..\\testdir\\test4");
	if (exists(p) && is_directory(p)) {
		lss(p);
	}

	else {
		printf("ERROR: Path \'%ls\' does not exists or is not a directory. Please verify the path.", p.c_str());
	}

	return 0;
}