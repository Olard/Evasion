/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Projectile.h"

using namespace glm::core::type;
using namespace glm::core::function::geometric;

Projectile::Projectile(World *world, glm::core::type::dvec3 position, GLdouble radius) :
		Entity(position, radius), world(world), velocity(PROJECTILE_SPEED) {
}

Projectile::~Projectile() {
}

void Projectile::update() {
	position = normalize(position) * (length(position) + velocity);
	world->sendToAll(getUpdateCommand());
}

Command Projectile::getNewCommand() const {
	Command newProjectile("NewProjectile");
	newProjectile.add(id);
	newProjectile.add(position.x);
	newProjectile.add(position.y);
	newProjectile.add(position.z);
	newProjectile.add(radius);
	newProjectile.add(velocity);

	return newProjectile;
}

Command Projectile::getUpdateCommand() const {
	Command newProjectile("Projectile");
	newProjectile.add(id);
	newProjectile.add(position.x);
	newProjectile.add(position.y);
	newProjectile.add(position.z);
	newProjectile.add(radius);
	newProjectile.add(velocity);

	return newProjectile;
}
