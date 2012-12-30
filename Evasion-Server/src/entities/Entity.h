/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef Entity_h
#define Entity_h

#include <glm/glm.hpp>	// Header for vectors
#include <GL/gl.h>		// OpenGL header for datatypes

#include "../network/Command.h"

class Entity {
	/**
	 * Fields
	 */
	public:
		glm::core::type::dvec3 position;
		GLdouble radius;
		unsigned id;
	protected:
		static unsigned id_counter;
	/**
	 * Constructors / Destructor
	 */
	public:
		Entity();
		/**
		 * Standard constructor
		 *
		 * @param position the position of the entity
		 * @param radius the radius of the entity
		 */
		Entity(glm::core::type::dvec3 position, GLdouble radius);
		virtual ~Entity();
	/**
	 * Methods
	 */
	public:
		/**
		 * Returns a command that tells the client to delete the entity
		 */
		Command getDeleteCommand() const;

		/**
		 * An update function that is pure virtual for all the entities
		 */
		virtual void update() = 0;
};

#endif
