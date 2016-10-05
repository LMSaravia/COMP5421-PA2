#include <algorithm>
#include <istream>
#include <string>
#include "Command.h"

using namespace std;

Command::Command(size_t size, int curr) : rangeStart(curr), hasSeparator(false), rangeEnd(curr), letter('p'),
maxRange(size), valid(false)
{
	//cout << "Command ctor" << endl;
}

Command::Command(const Command & source) : rangeStart(source.rangeStart), hasSeparator(source.hasSeparator),
rangeEnd(source.rangeEnd), letter(source.letter), maxRange(source.maxRange),valid(source.valid)
{ 
	//cout << "Command copy ctor" << endl;
}

Command::~Command()
{
	//cout << "Command dtor" << endl;
}

void Command::operator=(const Command & rhs)
{
	//cout << "Command assignment operator=" << endl;
	rangeStart = rhs.getRangeStart();
	hasSeparator = rhs.hasSeparator;
	rangeEnd = rhs.getRangeEnd();
	letter = rhs.getLetter();
	valid = rhs.isValid();
}

// Accessors and Mutators

int Command::getRangeStart() const
{
	return rangeStart;
}

int Command::getRangeEnd() const
{
	return rangeEnd;
}

char Command::getLetter() const
{
	return letter;
}

bool Command::isValid() const
{
	return valid;
}

//void Command::setLetter(char c)
//{
//	string temp{ c };
//	if (temp.find_first_of("acdinpqruw=") != -1)
//		letter = c;
//	else
//	{
//		cerr << "Error. Invalid command letter: " << c << endl;
//		setValid(false);
//	}
//}

// other methods
/*takes a command line and breaks it down into its component parts*/
void Command::parse()
{
	string line{ "" };	// holds a line of input
	int letterIndex{ -1 }; // holds command letter index

	cout << ":";
	getline(cin, line);
	removeBlanks(line);

	// start parsing
	if (!line.empty())
	{	// find command letter
		letterIndex = findLetterIndex(line);

		if (letterIndex == -1) // has range only, use full line to get range.
		{
			//cout << "Has range only " << endl;
			// filter invalid chars
			if (line.find_first_not_of(".,$1234567890") == string::npos)
				parseRange(line);
			else // there is an invalid char.
				letter = 'z';
		}
		else if (letterIndex == 0) // has letter command only, use only line[0]
		{
			//cout << "Has letter command only " << line[letterIndex] << endl;
			letter = line[letterIndex];
		}
		else // has range and letter command, use first part of line to get range
		{
			//cout << "Has range " << line.substr(0, letterIndex) <<
				//" and letter command " << line[letterIndex] << endl;
			letter = line[letterIndex];
			// filter invalid chars
			if (line.substr(0, letterIndex).find_first_not_of(".,$1234567890") == string::npos)
				parseRange(line.substr(0, letterIndex));
			else // there is an invalid char.
				letter = 'z';
		}
	}
	else {	// just pressed enter. Same as 1d
		rangeEnd = 1;
		letter = 'd';
	}
	// Turn valid flag on if it is a valid command
	validate();
}

inline void  Command::removeBlanks(std::string &s)
{
	s.erase(remove(s.begin(), s.end(), '\t'), s.end());
	s.erase(remove(s.begin(), s.end(), ' '), s.end());
}

size_t Command::findLetterIndex(const string &s) const
{
	if (s.find_first_of("befghjklmostvxyz") != -1)
		cerr << "Error. Invalid letter detected." << endl;
	return s.find_first_of("acdinpqruw=");
}

/*takes a non-empty substring containing only the range section*/
void Command::parseRange(const string &s)
{
	//cout << "Inside parseRange()" << endl;
	//cout << "String s is: " << s << endl;
	int separatorIndex{ -1 };
	separatorIndex = findSeparatorIndex(s);
	//cout << "Separator index is: " << separatorIndex << endl;

	if (separatorIndex == -1) // no semicolon, just y
	{ 
		//cout << "Has y" << endl;
		hasSeparator = false;
		if (s == "."); // cout << " y is '.'. Uses current by default" << endl;
		else if (s == "$")
		{
			rangeStart = maxRange;
			rangeEnd = maxRange;
		}
		else
		{
			rangeStart = stoi(s);
			rangeEnd = stoi(s);
		}
	}
	else if (separatorIndex == 0) // has ,?
	{ 
		hasSeparator = true;
		if (s.length() == 1) // has separator only, no place for y
		{ 
			//cout << "Has semicolon only" << endl;
			rangeStart = 1;
			rangeEnd = maxRange;
		}
		else	// has ,y
		{
			//cout << "Has ,y" << endl;
			if (s.substr(1) == "$")
				rangeEnd = maxRange;
			else
				rangeEnd = stoi(s.substr(1));
		}
	}
	else if (separatorIndex > 0)  // has x,?
	{	// look for x
		//cout << "Has x,_ " << endl;
		hasSeparator = true;
		if (s.substr(0, separatorIndex) == "."); // cout << " x is '.'. Uses current by default" << endl;
		else if (s.substr(0, separatorIndex) == "$")
		{
			rangeStart = maxRange;
			rangeEnd = maxRange;
		}
		else
		{
			rangeStart = stoi(s.substr(0, separatorIndex));
		}
		// look for y
		if (separatorIndex != s.length() - 1) // if ',' is not the last char, it has y
		{ 
			//cout << "Has x,y" << endl;
			if (s.substr(separatorIndex + 1) == "."); // cout << "y is '.'. Uses current by default" << endl;
			else if (s.substr(separatorIndex + 1) == "$")
				rangeEnd = maxRange;
			else
				rangeEnd = stoi(s.substr(separatorIndex + 1));
		}
	}
}

size_t Command::findSeparatorIndex(string const & s) const
{
	return s.find_first_of(",");
}

void Command::validate() {
	//cout << "Inside validate()" << endl;
	if (maxRange == 0)	// check for valid letter only
	{
		switch (letter)
		{
		case 'a':
		case 'i':
		case 'u':
		case 'd':
		case 'r':
		case 'p':
		case 'n':
		case 'c':
		case 'q':
		case 'w':
		case '=':
			valid = true;
			break;
		default:
			//cout << "inside default case (valid = false)" << endl;
			valid = false;
		}
	}
	else 
	{
		switch (letter)
		{// a, i , u , d use only (y)
		case 'a':
		case 'i':
			if (rangeEnd > 0 && rangeEnd <= maxRange)
				valid = true;
			else
				cout << "Invalid range " << rangeEnd << endl;
			break;
		case 'u':
		case 'd':
			if (rangeEnd > 0) // && rangeEnd < (maxRange - 1))
				valid = true;
			else
				cerr << "Invalid range " << rangeEnd << endl;
			break;
			// r, p, n, c use (x,y)
		case 'r':
		case 'p':
		case 'n':
		case 'c':
		//	cout << "inside rpnc case" << endl;
			if (rangeStart > 0 && rangeStart <= maxRange && rangeStart <= rangeEnd
				&& rangeEnd > 0 && rangeEnd <= maxRange)
				valid = true;
			else
				cerr << "error: invalid range " << rangeStart << " through " << rangeEnd << endl;
			break;
			// w and = don't use range
		case 'q':
		case 'w':
		case '=':
			valid = true;
			break;
		default:
			//cout << "inside default case (valid = false)" << endl;
			valid = false;
		}
	}
}
