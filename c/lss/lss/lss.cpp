#include "lss.h"
#include <iterator>
#include <algorithm>


void lss::lss(char * pathname)
{
	path p(pathname);
	if (exists(p) && is_directory(p)) {
		static map<string, BaseGroups> filebasemap;

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
				filenumbers[pos] = { 0, iter->str(), stoi(iter->str()) };
			}

			if (filebasemap.find(basename) == filebasemap.end()) {
				list<BaseNumberSet> basefilenumbers = { BaseNumberSet(filename, filenumbers) };
				BaseGroups groups(basefilenumbers, list<Numbers_t>());
				filebasemap.insert({ basename, groups });
			}
			else {
				bool Found = false;
				for (auto & base : filebasemap[basename].Bases) {
					Found = (base.IncrementBaseSet(filenumbers)) ? true : false;
				}
				if (Found) {
					filebasemap[basename].Files.push_back(filenumbers);
				}
				else {
					BaseNumberSet B(filename, filenumbers);
					for (auto file : filebasemap[basename].Files) {
						B.IncrementBaseSet(file);
					}
					filebasemap[basename].Bases.push_back(B);
				}
			}
		}

		for (auto filename : filebasemap) {
			filename.second.print();
		}
	}
	else {
		printf("ERROR: Path \'%ls\' does not exists or is not a directory. Please verify the path.", p.c_str());
	}
}

string generateRangeStr(list<int> & filelist)
{
	string range = "";
	vector<int> diff, mylist;
	filelist.sort();
	copy(begin(filelist), end(filelist), back_inserter(diff));
	copy(begin(filelist), end(filelist), back_inserter(mylist));

	adjacent_difference(filelist.begin(), filelist.end(), diff.begin());

	vector<size_t> idx(filelist.size());
	iota(idx.begin(), idx.end(), 0);
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


bool operator !=(const NumberSet & lhs, const NumberSet & rhs) {
	if (lhs.strnum != rhs.strnum) {
		return true;
	}
	return false;
}

bool operator !=(const Numbers_t & lhs, const Numbers_t & rhs) {
	for (auto i : lhs) {
		if (lhs.find(i.first)->second != rhs.find(i.first)->second) {
			return true;
		}
	}
	return false;
}

BaseNumberSet::BaseNumberSet(string basename, Numbers_t & set) :
BaseName(basename), Count(1)
{
	FileNumbersMap = set;
}

string BaseNumberSet::asPaddedString()
{
	string outputname = BaseName;
	int highestposition = getHighestPositionCount();
	for (auto position : FileNumbersMap)
	{
		string str = position.second.strnum;
		int strsize = str.size();
		if (position.first == highestposition) {
			string padding = (strsize > 1) ? "%0" + to_string(strsize) + "d" : "%d";
			outputname.replace(position.first, strsize, padding);
		}
		else {
			outputname.replace(position.first, strsize, str);
		}		
	}
	return outputname;
}

int BaseNumberSet::getHighestPositionCount()
{
	int highestcount = 0;
	int highestposition = 0;
	for (auto position : FileNumbersMap) {
		if ((position.second.positioncount > highestcount) ||
			((position.second.positioncount == highestcount &&
				position.first > highestposition)))
		{
			highestcount = position.second.positioncount;
			highestposition = position.first;
		}
	}
	return highestposition;
}

bool BaseNumberSet::isIncrementOf(Numbers_t & set, int position) {
	for (auto i : FileNumbersMap) {
		if (position == i.first) continue;
		if (i.second != set[(int(i.first))]) {
			return false;
		}
	}
	return true;
}

bool BaseNumberSet::IncrementBaseSet(const Numbers_t & set) {

	list<int> count = {};
	for ( auto position : FileNumbersMap) {
		if (position.second != set.find(position.first)->second) {
			count.push_front(position.first);
		}
	}
	if (count.size() == 1) {
		++Count;
		FileNumbersMap[count.front()].positioncount++;
		return true;
	}
	return false;
}

const NumberSet BaseNumberSet::operator[](int position) {
	const NumberSet & ns = FileNumbersMap[position];
	return ns;
}


void BaseGroups::print() {
	Bases.sort(compareCounts);
	for (auto base : Bases) {
		string basename = base.asString();
		string sequencerange = "";
		int pos = base.getHighestPositionCount();
		list<int> filelist = { base[pos].intnum };
		auto file = Files.begin();
		while (file != Files.end()) {
			if (base.isIncrementOf((*file), pos)) {
				filelist.push_back((*file)[pos].intnum);
				file = Files.erase(file);
			}
			else {
				++file;
			}
		}

		if (filelist.size() > 1) {
			basename = base.asPaddedString();
			sequencerange = generateRangeStr(filelist);
		}
		basename.insert(basename.size(), 25 - basename.size(), ' ');
		cout << filelist.size() << " " << basename << " " << sequencerange << endl;
	}
}