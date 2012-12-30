/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

#include "World.h"

using namespace std;
using namespace glm::core::type;
using namespace glm::core::function::geometric;

oogl::Texture* World::texture = 0;

World::World(GLfloat radius)
	: Entity(glm::core::type::dvec3(), radius) {
	if (texture == 0) {
		texture = oogl::loadTexture("data/world.jpg");
	}
	qobj = gluNewQuadric();
}
World::~World() {
	gluDeleteQuadric(qobj);
}

void World::draw() {
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(position.x, position.y, position.z);
		gluQuadricTexture(qobj,GL_TRUE);

		glEnable(GL_TEXTURE_2D);
		texture->bind();

		gluSphere(qobj, radius, 2000, 2000);

		glDisable(GL_TEXTURE_2D);
		texture->unbind();
		for (list<Entity*>::const_iterator iterator = entities.begin(), end = entities.end(); iterator != end; ++iterator) {
			glPushMatrix();
				(*iterator)->draw();
			glPopMatrix();
		}
}


void World::update() {
	for (list<Entity*>::const_iterator iterator = entities.begin(), end = entities.end(); iterator != end; ++iterator) {
		(*iterator)->update();
	}
}
