/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <list>
#include <string>

/**
 * This class encapsulates the commands that are sent to a client.
 */
class Command {
	/** Fields */
	private:
		std::string command;
		std::list<std::string> parameters;
	/** Constructors / Destructor */
	public:
		/*
		 * Initializes the command with the given command string
		 *
		 * @param command the command string
		 */
		Command(std::string command);
		virtual ~Command();

	/** Methods */
	public:
		/**
		 * Returns the parameter on the given index
		 *
		 * @param index the index of the wanted parameter
		 */
		std::string getParameter(unsigned index) const;
		/** Returns the Command for transmission in from of "command(Param1,param2)" */
		std::string getCommand() const;

		/** Two commands are equal when there command string before the '(' is the same */
		bool operator==(const std::string &other) const;
		bool operator==(const char* other) const;

		/** Adds the given value to the command (seperated by a ',') */
		Command& add(std::string &param);
		Command& add(float param);
		Command& add(double param);
		Command& add(unsigned param);
		Command& add(int param);
		Command& add(Command &param);
		Command& add(const char* param);
};

#endif /* COMMAND_H_ */
