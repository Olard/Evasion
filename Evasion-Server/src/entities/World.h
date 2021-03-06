/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef World_h
#define World_h

#include <list>
#include "Entity.h"
class Client;

class World : public Entity {
	/**
	 * Fields
	 */
	public:
		std::list<Entity*> entities;
		std::list<Client*> clients;
	/**
	 * Constructor / Destructor
	 */
	public:
		/**
		 * Creates the world with the given radius
		 *
		 * @param the radius
		 */
		World(GLfloat radius = 100);
		virtual ~World();
	/**
	 * Methods
	 */
	public:
		/**
		 * Sends a command to all clients in the world.
		 *
		 * @param command the command to be sent
		 */
		void sendToAll(const Command &command) const;

		/**
		 * Updates all Subentities of the world
		 */
		virtual void update();
};


#endif
