#ifndef COMMAND_H
#define COMMAND_H

#include<iostream>
#include<string>

/*Represents a command and is in charge of parsing that command into its
components*/
class Command {
public:
	Command() = delete;
	Command(std::size_t size, int curr);
	explicit Command(const Command & source);
	~Command();
	void operator=(const Command & rhs);
	int getRangeStart() const;
	int getRangeEnd() const;
	char getLetter() const;
	bool isValid() const;
	/*Takes a command line from user and breaks it down into its component parts*/
	void parse();

private:
	int rangeStart;		// start address in line range
	bool hasSeparator;	// uses address separator
	int rangeEnd;		// end address in line range
	char letter;		// the command letter
	int maxRange;		// holds the max range allowed (buffer size)
	bool valid;			// flag indicating the command is valid
	
	/*Removes spaces and tabs from a string*/
	inline void removeBlanks(std::string &);
	/*Finds command letter on string and returns its position. Returns -1 if not found*/
	inline size_t findLetterIndex(const std::string &) const;
	/*takes range part of a string and breaks it into its component parts*/
	void parseRange(const std::string &);
	/*Finds a separator (,) on a string and returns its position. Returns -1 if not found*/
	inline size_t findSeparatorIndex(const std::string & s) const;
	/*sets valid flag to true if command has a valid range letter*/
	void validate();

}; // end of class Command

#endif // !COMMAND_H
