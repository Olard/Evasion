/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Generation.h"
#include "entities/Obstacle.h"


using namespace glm::core::type;
using namespace glm::core::function::geometric;

dvec3 generateDirection() {
	return normalize(dvec3(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2));
}

GLdouble generateRadius(GLdouble min, GLdouble max) {
	return min + (max - min) * rand() / RAND_MAX;
}

World* generateWorld(GLdouble radius = 200, unsigned obstacleCount = 0) {
	World *world = new World(radius);

	for (unsigned i = 0; i < obstacleCount; ++i) {
		Obstacle *obstacle = new Obstacle(generateDirection() * world->radius, generateRadius(5, 35));
		world->entities.push_back(obstacle);
	}

	return world;
}
