/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#ifndef World_h
#define World_h

#include "Entity.h"
#include <oogl/Texture.h>
#include <list>

class World : public Entity {
	/**
	 * Fields
	 */
	public:
		std::list<Entity*> entities;
	private:
		static oogl::Texture* texture;
		GLUquadric *qobj;
	/**
	 * Constructor / Destructor
	 */
	public:
		/**
		 * Initializes the world with the given radius.
		 *
		 * @param radius the worldradius
		 */
		World(GLfloat radius = 100);
		virtual ~World();
	/**
	 * Methods
	 */
	public:
		/**
		 * Inherited from Entity
		 */
		virtual void draw();

		/**
		 * Inherited from Entity. Calls the update method of all entities in the world.
		 */
		virtual void update();
};

#endif
