/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 */

// I/O stuff for configuration
#include <iostream>
#include <fstream>

// OpenGL Headers
#include <GLee.h>
#include <GL/glut.h>
#include <oogl/gl_error.h>

// Network and
#include <process.h>
#include "network/ClientSocket.h"
#include "network/Command.h"

// Entities
#include "entities/World.h"
#include "entities/Player.h"
#include "entities/Obstacle.h"
#include "entities/Projectile.h"

using namespace std;
using namespace glm::core::type;
using namespace glm::core::function::geometric;

World *world;
Player *player;
ClientSocket *serverConnection;

// Is set to true while the key is down
bool keyMapping[256];
bool specialKeyMapping[256];

int windowWidth, windowHeight;


void initLighting() {
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components
	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { 0, 0, 0, 0 };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glShadeModel(GL_SMOOTH);

}

/**
 * Calculates whether there are any collisions with the player and adjusts the players velocity-vector.
 */
void handleCollisions() {
	for (list<Entity*>::const_iterator iterator = world->entities.begin(), end = world->entities.end(); iterator != end; ++iterator) {
		if ((*iterator)->id != player->id) {
			// The player collides when the radius intersects with another radius
			if (length((*iterator)->position - player->position) < (player->radius + (*iterator)->radius)) {
				// If the object the player collides with is a projectile the hp are lowered
				Projectile *projectile = dynamic_cast<Projectile*> (*iterator);
				if (projectile) {
					Command hpCommand("Hp");
					hpCommand.add(projectile->id);
					serverConnection->sendLine(hpCommand.getCommand());
				}

				// Calculate the collision normal
				dvec3 collisionVector = player->position - (*iterator)->position;

				// If the collisionVector is too small we normalize it
				if (length(collisionVector) < 0.1) {
					collisionVector = -player->direction;
				} else {
					collisionVector = normalize(collisionVector);
				}

				// Set the players position to the border of the colliding object
				player->position = (*iterator)->position + (collisionVector * ((*iterator)->radius + player->radius));

				// Mirror and reverse the velocity over the collisionvector
				dvec3 newVelocity = (player->velocity - collisionVector * (2 * dot(player->velocity, collisionVector))) * 0.8;

				// If there is an other player push him a little
				Player* otherPlayer = dynamic_cast<Player*> ((*iterator));
				if (otherPlayer) {
					newVelocity += otherPlayer->velocity * 0.5;
				}

				// This it to ignore NaN values
				if (newVelocity.x == newVelocity.x) {
					player->velocity = newVelocity;
				}
			}
		}
	}
}

/**
 * Thread with the connection to the server.
 * Responds to the commands received by the server.
 */
unsigned __stdcall connector(void* parameter) {
	try {
		Socket* socket = (Socket*) parameter;

		while (1) {
			string message = socket->receiveLine();
			while (message != "" && message != "\n") {
				Command serverCommand(message);
				if (serverCommand == "World") {
					world->radius = atoi(serverCommand.getParameter(0).c_str());
				} else if (serverCommand == "You") {
					player->parseUpdate(serverCommand);
				} else if (serverCommand == "NewPlayer") {
					Player *newPlayer = new Player(world);
					newPlayer->parseUpdate(serverCommand);
					world->entities.push_back(newPlayer);
				} else if (serverCommand == "Player") {
					unsigned id = atoi(serverCommand.getParameter(0).c_str());
					for (list<Entity*>::const_iterator iterator = world->entities.begin(), end = world->entities.end(); iterator != end; ++iterator) {
						if ((*iterator)->id == id) {
							Player *playerEntity = dynamic_cast <Player*> (*iterator);
							if (playerEntity) {
								playerEntity->parseUpdate(serverCommand);
							}
						}
					}
				} else if (serverCommand == "Delete") {
					unsigned id = atoi(serverCommand.getParameter(0).c_str());
					for (list<Entity*>::const_iterator iterator = world->entities.begin(), end = world->entities.end(); iterator != end; ++iterator) {
						if ((*iterator)->id == id) {
							Entity* deletedObject = (*iterator);
							world->entities.remove(deletedObject);
							delete deletedObject;
							break;
						}
					}
				} else if (serverCommand == "Hp") {
					player->hp = atoi(serverCommand.getParameter(0).c_str());
				} else if (serverCommand == "NewObstacle") {
					Obstacle* obstacle = new Obstacle();
					obstacle->parseUpdate(serverCommand);
					world->entities.push_back(obstacle);
				} else if (serverCommand == "NewProjectile") {
					Projectile* projectile = new Projectile();
					projectile->parseUpdate(serverCommand);
					world->entities.push_back(projectile);
				} else if (serverCommand == "Projectile") {
					unsigned id = atoi(serverCommand.getParameter(0).c_str());
					for (list<Entity*>::const_iterator iterator = world->entities.begin(), end = world->entities.end(); iterator != end; ++iterator) {
						if ((*iterator)->id == id) {
							Projectile *projectile = dynamic_cast <Projectile*> (*iterator);
							if (projectile) {
								projectile->parseUpdate(serverCommand);
							}
						}
					}
				} else {
					cout << "Received unknown: " << message << endl;
				}
				message = socket->receiveLine();
			}
		}
	} catch(NetworkException &ex) {
		cout << "NetworkException Occured: " << ex.what() << endl;
	}

	return 0;
}

void init() {
	// enable lighting
	initLighting();

	world = new World(500);
	player = new Player(world);
	world->entities.push_back(player);
	Obstacle::initTexture();
	Projectile::initTexture();

	/*
	 * Read the configuration
	 */
	string server;
	int port = -1;

	const char *filename = "connection.txt";
	ifstream propertyFile (filename);

	if (propertyFile.is_open()) {
		string line;
		getline(propertyFile, line);
		int splitter = line.find(":");
		if (splitter > 0) {
			server = line.substr(0, splitter);
			port = atoi(line.substr(splitter + 1, line.size() - (splitter + 1)).c_str());
		}
	}

	propertyFile.close();

	if (port == -1) {
		cout << "Serveradress:";
		cin >> server;
		cout << "Port:";
		cin >> port;
	}

	bool worked = false;
	while (!worked) {
		try {
			serverConnection = new ClientSocket(server, port);
			worked = true;
		} catch (NetworkException &ex) {
			cout << "Could not connect to server (" << server << ":" << port << ")." << endl;
			cout << "Serveradress:";
			cin >> server;
			cout << "Port:";
			cin >> port;
		}
	}

	// Write the configuration of the server back to the file
	fstream outFile(filename, ios::out);
	outFile << server << ":" << port << endl;
	outFile.close();

	cout << "Connected" << endl;

	unsigned ret;
	_beginthreadex(0, 0, connector, (void*) serverConnection, 0, &ret);

	// set clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
}

/**
 * Sets the camera behind the player
 */
void setCamera() {
	dvec3 cameraPosition = player->position;	// Go to the players position
	cameraPosition -= (normalize(player->direction) * 30.0);	// Translate behind the player
	cameraPosition -= (normalize(cameraPosition) * 10.0);	// And at last get a little bit up to be inside the World

	dvec3 cameraLookat = normalize(player->position) * (world->radius - 15);

	dvec3 cameraUp = normalize(-cameraPosition);

	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			cameraLookat.x, cameraLookat.y, cameraLookat.z,
			cameraUp.x, cameraUp.y, cameraUp.z);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffer

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, windowWidth/((float)windowHeight), 1.0, 600.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	world->draw();


	LOG_GL_ERRORS();
	glutSwapBuffers(); // draw scene
}

int winId;

/**
 * Timed update function which is called as specified in glutTimerFunc
 *
 * @param value can be set in the glutTimerFunc (is not used here)
 */
void update(int value) {
	if (keyMapping['w'] || keyMapping['W']) { // Accelerate
		player->acceleration = PLAYER_ACCELERATION;
	} else if (keyMapping['s'] || keyMapping['S']) { // Accelerate backwards
		player->acceleration = -PLAYER_ACCELERATION / 2;
	}
	if (keyMapping['a'] || keyMapping['A']) { // Turn left
		dvec3 left = normalize(cross(-player->position, player->direction));
		player->direction = normalize((player->direction * (length(player->velocity) / 2.0 + 20.0)) + left);

	}
	if (keyMapping['d'] || keyMapping['D']) { // Turn right
		dvec3 right = normalize(cross(player->direction, -player->position));
				player->direction = normalize((player->direction * (length(player->velocity) / 2.0 + 20.0)) + right);
	}

	if (keyMapping[' ']) { // Hand-brake
		GLdouble speed = length(player->velocity);
		if (speed < 0.7) {
			player->velocity *= 0.01;
		} else if (speed < 1.3){
			player->velocity *= 0.1;
		} else if (speed < 1.7){
			player->velocity *= 0.6;
		}  else if (speed < 2.5){
			player->velocity *= 0.7;
		} else if (speed < 5.0)  {
			player->velocity *= 0.80;
		} else {
			player->velocity *= 0.90;
		}
	}

	if (player->hp == 0 || player->hp >= 1000) { // You lose and disconnect when you have 0 hp left
		glutDestroyWindow(winId);
		cout << "You have 0 hp left!" << endl;
		Sleep(3000);
		exit(0);
	}

	world->update();
	handleCollisions();

	if (serverConnection != 0) {
		serverConnection->sendLine(player->getPosition().getCommand());
	}

	glutPostRedisplay();
	glutTimerFunc(30, update, 0); //request to call again in at least 30ms
}

/**
 * This funciton is called when the window is resized
 *
 * @param w the width
 * @param h the height
 */
void reshape(int w, int h) {

	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, w/(float)h, 1.0, 600.0);
}

/**
 * Function that is called when nothing is to do
 */
void idle() {
	glutPostRedisplay(); //force a redisplay
}

/**
 * Function that handles the keydown event
 *
 * @param key the pressed key
 * @param x the mouse x position
 * @param y the mouse y position
 */
void keyPress(unsigned char key, int x, int y) {
	switch (key) {
	case 27://27=esc
		exit(0);
		break;
	default:
		keyMapping[key] = true;
		break;
	}
}

/**
 * Function that handles the keydown event of the special chars
 *
 * @param key the pressed key
 * @param x the mouse x position
 * @param y the mouse y position
 */
void specialKeyPress(int key, int x, int y) {
	specialKeyMapping[key] = true;
	keyMapping['w'] = specialKeyMapping[GLUT_KEY_UP];
	keyMapping['s'] = specialKeyMapping[GLUT_KEY_DOWN];
	keyMapping['a'] = specialKeyMapping[GLUT_KEY_LEFT];
	keyMapping['d'] = specialKeyMapping[GLUT_KEY_RIGHT];
}

/**
 * Function that handles the keyup event for special keys
 *
 * @param key the key that went up
 * @param x the x mouse position
 * @param y the y mouse position
 */
void specialKeyUp(int key, int x, int y) {
	specialKeyMapping[key] = false;
	keyMapping['w'] = specialKeyMapping[GLUT_KEY_UP];
	keyMapping['s'] = specialKeyMapping[GLUT_KEY_DOWN];
	keyMapping['a'] = specialKeyMapping[GLUT_KEY_LEFT];
	keyMapping['d'] = specialKeyMapping[GLUT_KEY_RIGHT];
}

/**
 * Function that handles the keyup event
 *
 * @param key the key that went up
 * @param x the x mouse position
 * @param y the y mouse position
 */
void keyUp (unsigned char key, int x, int y) {
	keyMapping[key] = false;
}

/**
 * Initializes the glut functions
 *
 * @param argc the number of arguments
 * @param argv the commandline arguments
 */
int setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 600);

	glutInitWindowPosition(100, 100);
	int windowId = glutCreateWindow("Evasion");

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardUpFunc(keyUp);
	glutKeyboardFunc(keyPress);

	glutSpecialUpFunc(specialKeyUp);
	glutSpecialFunc(specialKeyPress);

	glutTimerFunc(25, update, 0);  //request to call in at least 25ms
	return windowId;
}

void shutdown() {
	delete world;
	delete player;
	delete serverConnection;
}

int main(int argc, char** argv) {
	winId = setupGLUT(argc, argv);

	init();

	glutMainLoop();

	shutdown();

	return 0;
}
