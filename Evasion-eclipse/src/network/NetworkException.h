/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */
#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H
#include <stdexcept>

class NetworkException : public std::runtime_error {
	public:
		NetworkException(std::string message) : std::runtime_error(message) { }
};

#endif
