#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <list>
#include <string>
#include "Command.h"

/*owns the storage buffer and operates on it*/
class LineEditor {
public:
	LineEditor() = delete;
	LineEditor(const LineEditor &) = delete;
	explicit LineEditor(std::string &);
	void run();

private:
	std::string &fname;
	std::list <std::string> buffer;
	std::string *current{ nullptr };	// points to current line
	int currentLine{ 0 };	// for the current line
	Command order{ 0, 0 };
	bool changed{ false };

	void append(int pos);
	void append();
	void change(int x, int y);
	void insert(int pos);	
	void moveDown(int pos);
	void moveUp(int pos);
	void numberedPrint(int x, int y);
	void print(int x, int y);
	/*Exits led. The user is prompted to save the buffer, if the buffer has been 
	modified since the last 'w' command, if any; otherwise, if the buffer has 
	been modified since the start of the editing session.*/
	void quit();
	void remove(int x, int y);
	void write(std::string & f);
}; // end of class LineEditor
#endif // !LINE_EDITOR_H
