/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Entity.h"

unsigned Entity::id_counter = 0;

Entity::Entity() : id(id_counter++){ }

Entity::Entity(glm::core::type::dvec3 position, GLdouble radius)
	: position(position), radius(radius), id(id_counter++) { }

Entity::~Entity() { }

Command Entity::getDeleteCommand() const {
	Command deleteCommand("Delete");
	deleteCommand.add(id);
	return deleteCommand;
}
