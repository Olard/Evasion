/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef GENERATION_H_
#define GENERATION_H_

#include "glm/glm.hpp"
#include "GL/gl.h"
#include "entities/World.h"

/*
 * Generates a random direction vector (normalized).
 */
glm::core::type::dvec3 generateDirection();

/*
 * Generates a random GLdouble in given range.
 *
 * @param min the minimum range
 * @param max the maximum range
 */
GLdouble generateRadius(GLdouble min, GLdouble max);

/*
 * Generates a world with given parameters.
 *
 * @param radius the worldradius
 * @param obstacleCount the number of the initial obstacles
 */
World* generateWorld(GLdouble radius, unsigned obstacleCount);

#endif /* GENERATION_H_ */
