/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Command.h"
#include <sstream>

using namespace std;

Command::Command(string command) {
	stringstream stream;
	stream << command;

	if ((int) command.find("(") == -1 ) {
		stream << "(" << ")";
	}

	this->command = stream.str();
}

Command::~Command() { }

Command& Command::add(std::string &param) {
	stringstream newCommand;
	newCommand << command.substr(0, command.size() - 1);

	if (command[command.size() - 2] != '(') {
		newCommand << ",";
	}

	newCommand << param << ")";

	command = newCommand.str();
	return *this;
}


Command& Command::add(float param) {
	stringstream converter;
	converter << param;
	string newParam(converter.str());
	return add(newParam);
}

Command& Command::add(double param) {
	stringstream converter;
	converter << param;
	string newParam(converter.str());
	return add(newParam);
}

Command& Command::add(unsigned param) {
	stringstream converter;
	converter << param;
	string newParam(converter.str());
	return add(newParam);
}

Command& Command::add(int param) {
	stringstream converter;
	converter << param;
	string newParam(converter.str());
	return add(newParam);
}

Command& Command::add(Command &param) {
	string newParam(param.getCommand());
	return add(newParam);
}



Command& Command::add(const char* param) {
	string newParam(param);
	return add(newParam);
}

string Command::getParameter(unsigned index) const {
	int startParam = command.find("(") + 1;
	// Inner commands should not have multiple params
	for (unsigned i = 0; i < index; ++i) {
		startParam = command.find(",", startParam) + 1;
	}

	int endParam = command.find(",", startParam + 1);

	if (endParam <= 0) {
		endParam = command.size() - 1;
	}

	return command.substr(startParam, endParam - startParam);
}

bool Command::operator==(const std::string &other) const {
	return command.substr(0, other.length()) == other;
}

bool Command::operator==(const char *cstr) const {
	string other(cstr);
	return command.substr(0, other.length()) == other;
}


string Command::getCommand() const {
	return command;
}
