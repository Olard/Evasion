/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Client.h"
#include <cassert>

using namespace std;
using namespace glm::core::type;
using namespace glm::core::function::geometric;

Client::Client(const Socket* socket, const World* world) :
		Entity(),  socket(socket), world(world), direction(dvec3(1, 0, 0)), velocity(dvec3(0, 0, 0)) {
	radius = PLAYER_SIZE;
	position = glm::core::type::dvec3(0, 0, 0);
	for (unsigned i = 0; i < PLAYER_HP; ++i) {
		hp[i] = 0;
	}
}

bool Client::loseHp(unsigned id) {
	for (unsigned i = 0; i < PLAYER_HP; ++i) {
		if (hp[i] == 0) {
			hp[i] = id;
			return true;
		} else if (hp[i] == id) {
			return false;
		}
	}
	return false;
}

unsigned Client::getCurrentHP() const {
	for (unsigned i = 0; i < PLAYER_HP; ++i) {
		if (hp[i] == 0) {
			return PLAYER_HP - i;
		}
	}
	return 0;
}

void Client::update() {
	position = normalize(position) * world->radius;
}


Command Client::getNewPlayer() const {
	Command newPlayer("NewPlayer");
	return addStatus(newPlayer);
}

Command& Client::addStatus(Command& command) const {
	command.add(this->id);
	command.add(this->position.x);
	command.add(this->position.y);
	command.add(this->position.z);
	command.add(this->direction.x);
	command.add(this->direction.y);
	command.add(this->direction.z);
	command.add(this->velocity.x);
	command.add(this->velocity.y);
	command.add(this->velocity.z);
	command.add(this->getCurrentHP());
	return command;
}

void Client::parseUpdate(Command& command) {
	this->id = atoi(command.getParameter(0).c_str());
	this->position.x = atof(command.getParameter(1).c_str());
	this->position.y = atof(command.getParameter(2).c_str());
	this->position.z = atof(command.getParameter(3).c_str());
	this->direction.x = atof(command.getParameter(4).c_str());
	this->direction.y = atof(command.getParameter(5).c_str());
	this->direction.z = atof(command.getParameter(6).c_str());
	this->velocity.x = atof(command.getParameter(7).c_str());
	this->velocity.y = atof(command.getParameter(8).c_str());
	this->velocity.z = atof(command.getParameter(9).c_str());
}

