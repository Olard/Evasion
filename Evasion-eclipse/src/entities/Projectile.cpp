/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Projectile.h"

using namespace glm::core::type;
using namespace glm::core::function::geometric;

oogl::Texture* Projectile::texture = 0;

void Projectile::initTexture() {
	if (texture == 0) {
			texture = oogl::loadTexture("data/obstacle.jpg");
	}
}

Projectile::Projectile() : Entity() {
	qobj = gluNewQuadric();
}

Projectile::~Projectile() {
	gluDeleteQuadric(qobj);
}

void Projectile::parseUpdate(Command& command) {
	this->id = atoi(command.getParameter(0).c_str());
	this->position.x = atof(command.getParameter(1).c_str());
	this->position.y = atof(command.getParameter(2).c_str());
	this->position.z = atof(command.getParameter(3).c_str());
	this->radius = atof(command.getParameter(4).c_str());
	this->velocity = atof(command.getParameter(5).c_str());
}

void Projectile::update() {
	// add the velocity to the position
	position = normalize(position) * (length(position) + velocity);
}

void Projectile::draw() {
	glTranslatef(position.x, position.y, position.z);
	gluQuadricTexture(qobj, GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);
	texture->bind();
	glColor3f(1.0, 0.5, 0.5);
	gluSphere(qobj, radius, 8, 8);
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	texture->unbind();
}
