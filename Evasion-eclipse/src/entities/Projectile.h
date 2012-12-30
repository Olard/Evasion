/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Entity.h"
#include "../network/Command.h"
#include <oogl/Texture.h>


class Projectile: public Entity {
	/**
	 * Fields
	 */
	private:
		static oogl::Texture* texture;
		GLUquadric *qobj;
		GLdouble velocity;
	/**
	 * Constructors / Destructor
	 */
	public:
		Projectile();
		virtual ~Projectile();
	/**
	 * Methods
	 */
	public:
		/**
		 * Inherited from Entity
		 */
		virtual void update();

		/**
		 * Inherited from Entity
		 */
		virtual void draw();

		/**
		 * Parses the command into the attributes of the projectile
		 *
		 * @param command the command to parse from
		 */
		void parseUpdate(Command& command);

		/**
		 * Initialises the texture that is used by projectiles
		 */
		static void initTexture();
};

#endif /* PROJECTILE_H_ */
