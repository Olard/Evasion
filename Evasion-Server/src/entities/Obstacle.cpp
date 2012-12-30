/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Obstacle.h"


Obstacle::Obstacle(glm::core::type::dvec3 position, GLdouble radius) :
	Entity(position, radius) {
}

Obstacle::~Obstacle() { }

Command Obstacle::getNewCommand() const {
	Command newObstacle("NewObstacle");
	newObstacle.add(id);
	newObstacle.add(position.x);
	newObstacle.add(position.y);
	newObstacle.add(position.z);
	newObstacle.add(radius);

	return newObstacle;
}

void Obstacle::update() { /* Obstacles don't change */ }
