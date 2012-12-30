/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "Player.h"

#include <GL/glut.h>	// GLUT header
#include "../shapes/Torus.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace glm::core::type;
using namespace glm::core::function::geometric;

oogl::Texture* Player::torusTexture = 0;
oogl::Texture* Player::rotorTexture = 0;

Player::Player(const World* world) :
		Entity(dvec3(0, -world->radius + PLAYER_SIZE, 0), PLAYER_SIZE), direction(
				dvec3(1, 0, 0)), acceleration(0.0), rotationAngle(0.0), hp(4), world(
				world) {
	if (!torusTexture) {
		torusTexture = oogl::loadTexture("data/player/torus.jpg");
	}

	if (!rotorTexture) {
		rotorTexture = oogl::loadTexture("data/player/rotor.jpg");
	}

	assert(world != NULL);
}

Player::~Player() {
}

GLdouble angle(const dvec3 &vec1, const dvec3 &vec2) {
	GLdouble product = dot(vec1, vec2) / (length(vec1) * length(vec2));
	return product == product ? acos(product) * 180.0 / M_PI : 0.0;
}

void Player::draw() {
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);

	dvec3 drawPosition = normalize(position) * (world->radius - radius / 2);
	glTranslatef(drawPosition.x, drawPosition.y, drawPosition.z);

	// Align the player to the world
	dvec3 rotationVector = cross(position, dvec3(0, 1, 0));
	glRotatef(180 - angle(position, dvec3(0.0, 1.0, 0.0)), rotationVector.x,
			rotationVector.y, rotationVector.z);

	glEnable(GL_TEXTURE_2D);
	torusTexture->bind();
	glRotatef(90, 1, 0, 0);
	drawTorus(0.5, radius, 30, 30);
	torusTexture->unbind();

	/**
	 * Rotor. A red rotor means a lost hitpoint.
	 */
	glPushMatrix();
	rotorTexture->bind();
	glRotatef(rotationAngle, 0, 0, 1);

	if (hp > 3) {
		glColor3f(0.0, 1.0, 0.0);
	} else {
		glColor3f(1.0, 0.0, 0.0);
	}

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 0);
	glVertex3f(radius, -1.0, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(radius, 1.0, 0);
	glEnd();

	if (hp > 2) {
		glColor3f(0.0, 1.0, 0.0);
	} else {
		glColor3f(1.0, 0.0, 0.0);
	}

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(-1.0, radius, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, radius, 0);
	glEnd();

	if (hp > 1) {
		glColor3f(0.0, 1.0, 0.0);
	} else {
		glColor3f(1.0, 0.0, 0.0);
	}

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 0);
	glVertex3f(-radius, -1.0, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-radius, 1.0, 0);
	glEnd();

	if (hp > 0) {
		glColor3f(0.0, 1.0, 0.0);
	} else {
		glColor3f(1.0, 0.0, 0.0);
	}

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-1.0, -radius, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, -radius, 0);
	glEnd();

	rotorTexture->unbind();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Draw a little locator over the player
	dvec3 locator = normalize(position) * (length(position) - 40.0);

	glTranslatef(locator.x, locator.y, locator.z);
	glColor3f(0.1, 0.1, 1.0);
	glutSolidSphere(2.0, 5, 5);
	glColor3f(1.0, 1.0, 1.0);

}

void Player::update() {
	velocity += (direction * acceleration);

	// If the player is to fast set the speed to the maximum speed
	if (length(velocity) > PLAYER_MAX_SPEED) {
		velocity = normalize(velocity) * PLAYER_MAX_SPEED;
	}

	position += velocity;

	velocity *= 0.99; // If you don't press a button the player gets slower
	acceleration *= 0.95;

	// Calculate the new direction vector
	dvec3 newDirection = cross(position, direction);
	direction = normalize(cross(newDirection, position));

	// Rotate the rotor
	rotationAngle += 10.0 + length(velocity) * 0.5;

	// Lift the player up to the worldradius
	position = (normalize(position) * world->radius);
}

void Player::parseUpdate(Command& command) {
	this->id = atoi(command.getParameter(0).c_str());
	this->position.x = atof(command.getParameter(1).c_str());
	this->position.y = atof(command.getParameter(2).c_str());
	this->position.z = atof(command.getParameter(3).c_str());
	this->direction.x = atof(command.getParameter(4).c_str());
	this->direction.y = atof(command.getParameter(5).c_str());
	this->direction.z = atof(command.getParameter(6).c_str());
	this->velocity.x = atof(command.getParameter(7).c_str());
	this->velocity.y = atof(command.getParameter(8).c_str());
	this->velocity.z = atof(command.getParameter(9).c_str());
	this->hp = atoi(command.getParameter(10).c_str());
}

Command Player::getPosition() const {
	Command positionCommand("Player");
	positionCommand.add(id);
	positionCommand.add(position.x);
	positionCommand.add(position.y);
	positionCommand.add(position.z);
	positionCommand.add(direction.x);
	positionCommand.add(direction.y);
	positionCommand.add(direction.z);
	positionCommand.add(velocity.x);
	positionCommand.add(velocity.y);
	positionCommand.add(velocity.z);
	return positionCommand;
}

