/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#ifndef Entity_h
#define Entity_h

#include <glm/glm.hpp>
#include <GLee.h>
#include <GL/gl.h>		// OpenGL header for datatypes

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
		 * @param the radius of the entity
		 */
		Entity(glm::core::type::dvec3 position, GLdouble radius);
		virtual ~Entity();
	/**
	 * Methods
	 */
	public:
		/**
		 * The pure virtual update method of every entity that is called to update the entity
		 */
		virtual void update() = 0;

		/**
		 * The pure virtual draw method.
		 */
		virtual void draw() = 0;
};

#endif
