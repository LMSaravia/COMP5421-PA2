#include<iostream>
#include<fstream>
#include<string>
#include "LineEditor.h"

using namespace std;

LineEditor::LineEditor(std::string & filename) : fname(filename)
{
	if (filename.empty())
	{
		cout << "\"?\" [New File]" << endl;
	}
	else
	{
		cout << "filename is: " << filename << endl;
		ifstream fin;
		fin.open(filename); // opens file for input

							// if filename does not exist 
		if (!fin)
		{
			cerr << "Unable to open file " << filename << endl;
			cout << "\"" << filename << "\" " << "[New File]" << endl;
		}
		else // if filename exists read from it into the buffer line by line
		{
			int lineCounter{ 0 };
			string line;
			while (getline(fin, line))
			{
				//cout << line << endl;
				buffer.push_back(line);
				++lineCounter;
				currentLine = lineCounter;
			}
			// Show file name and number of lines
			cout << "\"" << filename << "\" " << lineCounter << " lines" << endl;
			cout << "Current line is " << currentLine << endl;
		}

		// close stream
		fin.close();
	}
	

} // end of ctor

void LineEditor::run()
{
	cout << "Entering command mode." << endl;
	while (true)
	{
		Command temp{ buffer.size(), currentLine };
		temp.parse();
		if (temp.isValid())
		{
			order = temp;
			if (buffer.empty())
			{
				switch (order.getLetter())
				{
				case 'a':
				case 'i':
					append();
					break;
				case 'q':
					quit();
					break;
				default:
					cerr << "error. file empty - enter 'q' to quit, 'a' to append or 'i' to insert." << endl;
				}
			}
			else
			{
				switch (order.getLetter())
				{
				case 'a':
					append(order.getRangeEnd());
					break;
				case 'c':
					change(order.getRangeStart(), order.getRangeEnd());
					break;
				case 'd':
					moveDown(order.getRangeEnd());
					break;
				case 'i':
					insert(order.getRangeEnd());
					break;
				case 'n':
					numberedPrint(order.getRangeStart(), order.getRangeEnd());
					break;
				case 'p':
					print(order.getRangeStart(), order.getRangeEnd());
					break;
				case 'q': 
					quit();
					break;
				case 'r':
					remove(order.getRangeStart(), order.getRangeEnd());
					break;
				case 'u':
					moveUp(order.getRangeEnd());
					break;
				case 'w':
					write(fname);
					break;
				case '=':
					cout << currentLine << endl;
					break;
				default:
					break;
				} // end of switch
			} // end of else
		} 
		else // temp is not valid
		{
			cout << "Error. Invalid command." << endl;
		}
	} // end of while
} // end of run

void LineEditor::append(int pos)
{
	//cout << "inside append(" << pos << ")" << endl;
	if (buffer.empty() || pos == buffer.size()) // append to the list if empty or last position.
	{
		append();
	}
	else // not empty, and not last. Insert after position
	{
		//cout << "Enter a new line:" << endl;
		string line;
		auto it = buffer.begin();
		// move iterator to position after pos
		int i;
		for (i = 1; i <= pos; ++i) {
			it++;
		}
		getline(cin, line);
		while (cin.good() && line != ".")
		{
			buffer.insert(it, line);
			changed = true;
			i++;
			getline(cin, line);
		}
		// make sure that the current line is set to the last line appended
		currentLine = --i;
	}
}

void LineEditor::append()
{
	//cout << "inside append()" << endl;
	//cout << "Enter a new line:" << endl;
	string line;
	getline(cin, line);
	while (cin.good() && line != ".")
	{
		buffer.push_back(line);
		changed = true;
		getline(cin, line);
	}
	// set current line to the last line appended
	currentLine = buffer.size();
}

/*The current line address is set to the last line changed.*/
void LineEditor::change(int x, int y)
{
	//cout << "Inside change(" << x << ", " << y << ")" << endl;
	string target;
	string replacement;
	// prompt and read target text
	cout << "Change what?" << endl;
	getline(cin, target);
	// prompt and read replacement text
	cout << "To what?" << endl;
	getline(cin, replacement);
	// search for ocurrence and // change matched string
	int i = 1;
	int lastChanged = x;
	for (auto it = begin(buffer); it != end(buffer); it++, i++)
	{
		if (i >= x && i <= y) // on each string in range
		{
			//cout << "it is: " << *it << endl;
			//cout << "i is: " << i << endl;
			// look for an ocurrence
			if (it->find(target) != string::npos)
			{	// replace part of string that matches
				//cout << "inside if (it->find...)" << endl;
				it->replace(it->find(target), target.length(), replacement);
				lastChanged = i;
			}
		}
	}
	currentLine = lastChanged;
}

void LineEditor::insert(int pos)
{
	//cout << "inside insert(" << pos << ")" << endl;
	if (buffer.empty()) // append if empty
	{
		append();
	}
	else // not empty, insert before position
	{
		//cout << "Enter a new line:" << endl;
		string line;
		auto it = buffer.begin();
		// move iterator to position
		int i;
		for (i = 1; i < pos; ++i) {
			it++;
		}
		getline(cin, line);
		while (cin.good() && line != ".")
		{
			buffer.insert(it, line);
			changed = true;
			i++;
			getline(cin, line);
		}
		// make sure that the current line is set to the last line appended
		currentLine = --i;
	}
}

/* Moves current line address down by n lines, but never beyond the 1st line.*/
void LineEditor::moveDown(int nLines)
{
	//cout << "inside moveDown(" << nLines << ")" << endl;
	auto currentIter = begin(buffer); // iterator to current line
	for (int i = 1; i < currentLine; i++) // move to current line
		++currentIter;

	while (nLines > 0)
	{
		if (currentIter == --end(buffer))
		{
			cout << "EOF reached" << endl;
			break;
		}
		++currentIter;
		++currentLine;
		--nLines;
	}
}

/* Moves current line address up by n lines, but never beyond the 1st line.*/
void LineEditor::moveUp(int nLines)
{
	//cout << "inside moveUp(" << nLines << ")" << endl;
	auto currentIter = begin(buffer); // iterator to current line
	for (int i = 1; i < currentLine; i++) // move to current line
		++currentIter;

	while (nLines > 0)
	{
		if (currentIter == begin(buffer))
		{
			cout << "BOF reached" << endl;
			break;
		}
		--currentIter;
		--currentLine;
		--nLines;
	}
}

void LineEditor::numberedPrint(int x, int y)
{
	//cout << "inside numberedPrint(" << x << ", " << y << ")" << endl;
	auto it = buffer.begin();
	for (int i = 1; i <= y; ++i) {
		if (i >= x)
			cout << i << "\t" << *it << endl;
		it++;
	}
	currentLine = y;
}

void LineEditor::print(int x, int y)
{ 
	//cout << "inside print(" << x << ", " << y << ")" << endl;
	auto it = buffer.begin();
	int i;
	for (i = 1; i <= y; ++i) {
		if (i >= x)
			cout << *it << endl;
		it++;
	}
	currentLine = y;
}

/*Exits led. The user is prompted to save the buffer, if the buffer has been
modified since the last 'w' command, if any; otherwise, if the buffer has
been modified since the start of the editing session.*/
void LineEditor::quit()
{
	if (changed)
	{
		char save{ '\0' };
		while (true)
		{
			cout << "Save changes? Y/N:" << endl;
			cin >> save;
			if (save == 'Y' || save == 'y' || save == 'N' || save == 'n')
				break;			
		}
		if (save == 'Y' || save == 'y')
		{
			write(fname);
			exit(0);
		}
		else
			exit(0);
	}
	else
	{
		exit(0);
	}
}

void LineEditor::remove(int x, int y)
{
	//Prerequisite x has to be > 0

	//cout << "inside remove(" << x << ", " << y << ")" << endl;

	if (y == buffer.size()) 
	{
		for (int i = y; i >= x; i--)
			buffer.pop_back();
		currentLine = x - 1;
	}
	else {
		// create 2 iterators
		auto iterX = begin(buffer);
		auto iterY = begin(buffer);
		// move iterator x to x position
		advance(iterX, x - 1);
		// move iterator y to position after y
		advance(iterY, y);		
		// delete range (x,y)
		buffer.erase(iterX, iterY);
		currentLine = x;
	}
	changed = true;
}

void LineEditor::write(string &f)
{
	//cout << "inside write()" << endl;
	while(f.empty())
	{ 
		cout << "Please enter a file name:" << endl;
		cin >> f;
	}
	ofstream fout;
	fout.open(f);
	if (!fout)
	{
		cout << "File not found." << endl;
		exit(2);
	}

	for (auto it = begin(buffer); it != end(buffer); ++it)
		fout << *it << endl;
	
	changed = false;
}