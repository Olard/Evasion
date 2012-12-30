/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "World.h"
#include "Client.h"

using namespace std;
using namespace glm::core::type;

World::World(GLfloat radius)
	: Entity(dvec3(), radius) { }

World::~World() {
}

void World::sendToAll(const Command &command) const {
	for (list<Client*>::const_iterator iterator = clients.begin(), end = clients.end(); iterator != end; ++iterator) {
			(*iterator)->socket->sendLine(command.getCommand());
	}
}

void World::update() {
	for (list<Entity*>::const_iterator iterator = entities.begin(), end = entities.end(); iterator != end; ++iterator) {
		(*iterator)->update();
	}
}



