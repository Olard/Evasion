/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Obstacle.h"

using namespace std;

oogl::Texture* Obstacle::texture = 0;

void Obstacle::initTexture() {
	if (texture == 0) {
			texture = oogl::loadTexture("data/obstacle.jpg");
	}
}

Obstacle::Obstacle(glm::core::type::dvec3 position, GLdouble radius) : Entity(position, radius) {
	qobj = gluNewQuadric();
}

Obstacle::Obstacle() : Entity() {
	qobj = gluNewQuadric();
}


Obstacle::~Obstacle() {
	gluDeleteQuadric(qobj);
}

void Obstacle::parseUpdate(Command& command) {
	this->id = atoi(command.getParameter(0).c_str());
	this->position.x = atof(command.getParameter(1).c_str());
	this->position.y = atof(command.getParameter(2).c_str());
	this->position.z = atof(command.getParameter(3).c_str());
	this->radius = atof(command.getParameter(4).c_str());
}

void Obstacle::update() { /* Obstacles don't change */ }

void Obstacle::draw() {
	glTranslatef(position.x, position.y, position.z);

	glColor3f(0.75 , 0.75, 0.75);
	gluQuadricTexture(qobj, GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);
	texture->bind();
	gluSphere(qobj, radius, 8, 8);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	texture->unbind();
	glColor3f(1.0, 1.0, 1.0);
}
