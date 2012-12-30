/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "Entity.h"

class Obstacle: public Entity {
	/**
	 * Constructors / Destructor
	 */
	public:
		/**
		 * Standard constructor
		 *
		 * @param position the position of the obstacle
		 * @param radius the radius of the obstacle
		 */
		Obstacle(glm::core::type::dvec3 position, GLdouble radius);
		virtual ~Obstacle();
	/**
	 * Methods
	 */
	public:
		/**
		 * Returns the command that tells the clients to create a new obstacle.
		 */
		Command getNewCommand() const;

		/**
		 * The inherited update function
		 */
		virtual void update();
};

#endif /* OBSTACLE_H_ */
