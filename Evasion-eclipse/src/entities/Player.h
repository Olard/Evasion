/*
 * MultiMediaTechnology / QPT1 / �hlinger Bernhard
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#define PLAYER_SIZE 5

#include "Entity.h"

#include "World.h"
#include "../network/Command.h"
#include <oogl/Texture.h>

#define PLAYER_MAX_SPEED 8.0
#define PLAYER_ACCELERATION 0.5

class Player : public Entity {
	/**
	 * Fields
	 */
	public:
		glm::core::type::dvec3 velocity;
		glm::core::type::dvec3 direction;
		GLdouble acceleration;
		GLdouble rotationAngle;
		unsigned hp;
	private:
		static oogl::Texture* torusTexture;
		static oogl::Texture* rotorTexture;

		const World *world;
	/**
	 * Constructor / Destructor
	 */
	public:
		/**
		 * Standard constructor
		 *
		 * @param world the world the player is in
		 */
		Player(const World *world);
		~Player();
	/**
	 * Methods
	 */
	public:
		/**
		 * Returns the command that specifies the client's position
		 */
		Command getPosition() const;

		/**
		 * Inherited from Entity
		 */
		virtual void draw();

		/**
		 * Inherited from Entity
		 */
		virtual void update();

		/**
		 * Parses the the given command into the attribues of the player
		 *
		 * @param command the command to parse from
		 */
		void parseUpdate(Command& command);
};


#endif /* PLAYER_H_ */
