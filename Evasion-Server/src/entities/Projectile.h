/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#define PROJECTILE_SPEED 3.0

#include "World.h"

class Projectile: public Entity {
	/**
	 * Fields
	 */
	private:
		World* world;
		GLdouble velocity;
	/**
	 * Constructors / Destructor
	 */
	public:
		/**
		 * Standard constructor
		 *
		 * @param world the world in which the projectile is in
		 * @param position the position of the projectile
		 * @param radius the radius of the projectile
		 */
		Projectile(World *world, glm::core::type::dvec3 position, GLdouble radius);
		virtual ~Projectile();
	/**
	 * Methods
	 */
	public:
		/**
		 * Returns the command that tells clients to create a new projectile.
		 */
		Command getNewCommand() const;

		/**
		 * Returns the command that tells clients to update a projectile.
		 */
		Command getUpdateCommand() const;

		/**
		 * The update function which is inherited from the entity class.
		 */
		virtual void update();
};

#endif /* PROJECTILE_H_ */
