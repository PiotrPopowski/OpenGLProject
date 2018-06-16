/*
* Scene class. This class takes care of drawing all of the contents
* of our virtual aquarium as well as camera movement.
*
* This is a partial scene graph implementation where there is a list
* of objects to draw, the class sets up the environment and calls the
* draw method on each object in the queue.
*
*/

#include "Scene.h"

using namespace std;

int Scene::width;	/// the width of the window
int Scene::height;	/// the height of the window

					// light 0 colours
GLfloat Scene::ambient0[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat Scene::diffuse0[4] = { 0.4f, 0.4f, 0.4f, 1.01f };
GLfloat Scene::specular0[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat Scene::position0[4] = { 0.0f, -1.0f, 0.0f, 0.0f };

// light 1 colours
GLfloat Scene::ambient1[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat Scene::diffuse1[4] = { 0.45f, 0.45f, 0.45f, 1.0f };
GLfloat Scene::specular1[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat Scene::position1[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat Scene::direction1[4] = { 0.0f, 0.0f, -1.0f };

// spotlight cut-off angle
GLfloat Scene::spotAngle = 15.f;


/// Default Constructor. Initialises defaults.
Scene::Scene()
{
	cout << "-- Creating scene\n";

	error = GL_NO_ERROR;
	polygonModel = GL_FILL;
	elements = new std::list<Renderable*>;	// create our queue
	showMenu = true;	// menu is on
	light0On = false;	// light 0 is off
	light1On = true;	// light 1 is on
	fogMode = false;	// fog is off
	lightMode = false;	// lighting is off

						// set all counters to zero
	objects[OBJ_STONE] = 0;
	objects[OBJ_STARFISH] = 0;
	objects[OBJ_FISH] = 0;
	objects[OBJ_PLANT] = 0;

	// set up light 0 colours
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	// set up light 1 colours
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
}


/// Default destructor.
Scene::~Scene()
{
	cout << "++ Destructing scene\n";
	delete elements;
}


/// Renders a single frame of the scene
/*
* This function renders all of the objects that are attached
* to the rendering queue of the Scene.
*/
bool Scene::render(void)
{
	GLenum error;

	// check for errors and print them to stdout if any
	while ((error = glGetError()) != GL_NO_ERROR)
		cout << ">> Error: " << gluErrorString(error) << endl;

	// clear scene
	clear();

	// set up the miner's hat light before moving the camera
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);

	//position camera
	camera.position();

	// set up our directional overhead light
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	// check if there are any objects to draw
	if (elements->size() > 0)
	{
		// draw all elements in the scene
		iter = elements->begin();
		while (iter != elements->end())
			(*iter++)->draw();
	}

	drawHUD();

	glutSwapBuffers();
	return true;
}


/// Clears the scene for drawing
/*
* This method clears the scene for drawing. Both the colour
* and the depth buffers are cleared. The cameara position is
* also reset here.
*/
void Scene::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/// Adds an object to the rendering queue
/*
* This method simply adds on the passed object to the end
* of the rendering queue.
*/
void Scene::add(Renderable *object)
{
	elements->push_back(object);
}


/// Draws the heads up display
/*
* This method draws all of the required information about the
* current scene in the window. Orthographic projection is used
* here to position the text easier. both the PROJECTION and the
* MODELVIEW matrices are restored to their original states when
* this method returns.
*/
void Scene::drawHUD(void)
{
	// disable depth testing to HUD is drawn on top of everything
	glDisable(GL_DEPTH_TEST);

	// disable lightint so we see everything OK
	GLboolean lightsOn;
	glGetBooleanv(GL_LIGHTING, &lightsOn);
	glDisable(GL_LIGHTING);

	// grab and save the current projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// setup new projection
	glOrtho(-0.1f, 10.0f, 10.0f, -0.4f, -1.0f, 10.0f);

	// grab and save current view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);

	// print all of the stats
	printGL(0.0f, 0.0f, 0.0f, "Kamienie:    ", objects[OBJ_STONE]);
	printGL(1.5f, 0.0f, 0.0f, "Rozgwiazdy: ", objects[OBJ_STARFISH]);
	printGL(4.5f, 0.0f, 0.0f, "Ryby:     ", objects[OBJ_FISH]);
	printGL(6.0f, 0.0f, 0.0f, "Rosliny:   ", objects[OBJ_PLANT]);

	/*
	* the printGL() function expects the string to be 30 characters in length
	* so we have to pad each string with white space so that weird characters
	* do not appear
	*/




	// print the menu if needed
	if (showMenu)
		printMenu();

	// restore old view
	glPopMatrix();

	// grab and restore old projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// return to view mode
	glMatrixMode(GL_MODELVIEW);

	// enable depth testing again
	glEnable(GL_DEPTH_TEST);

	// enable lighting if needed
	if (lightsOn) glEnable(GL_LIGHTING);
}


/// Prints a string with a count attached
void Scene::printGL(GLfloat x, GLfloat y, GLfloat z, const char *str, int count)
{
	char buffer[30];
	for (int i = 0; i < 30; ++i)
		buffer[i] = ' ';

	sprintf(buffer, "%s %i", str, count);
	printGL(x, y, z, buffer);
}


/// Prints a string in the window
void Scene::printGL(GLfloat x, GLfloat y, GLfloat z, const char *str)
{
	glRasterPos3f(x, y, z);

	// print character one by one
	for (int j = 0; j < 30; j++) {
		int c = str[j];
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
	}
}


/// Displays the menu screen
void Scene::printMenu(void)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	printGL(0.0f, 1.7f, -1.0f, "---[ Klawiszologia ]------------");

	glColor3f(1.0f, 1.0f, 0.0f);
	printGL(0.0f, 2.3f, -1.0f, "F2    - Wygeneruj kamien             ");
	printGL(0.0f, 2.6f, -1.0f, "F4    - Wygeneruj rozgwiazde          ");
	printGL(0.0f, 3.9f, -1.0f, "F5    - Wygeneruj rybe              ");
	printGL(0.0f, 4.2f, -1.0f, "F6    - Wygeneruj rosline             ");

	glColor3f(0.5f, 1.0f, 0.0f);
	printGL(0.0f, 5.0f, -1.0f, "Underwater Scene by:             ");
	printGL(0.0f, 5.3f, -1.0f, "Przemyslaw Plociennik();         ");
	printGL(0.0f, 5.6f, -1.0f, "and Piotr Popowski();             ");

	glColor3f(0.0f, 1.0f, 0.0f);
	printGL(0.0f, 5.9f, -1.0f, "A/Z   - +/- kat widzenia          ");
	printGL(0.0f, 6.2f, -1.0f, "GORA    - Zbliz                    ");
	printGL(0.0f, 6.5f, -1.0f, "DOL  - Oddal                       ");
	printGL(0.0f, 6.8f, -1.0f, "LEWO  - Obroc w prawo               ");
	printGL(0.0f, 7.1f, -1.0f, "PRAWO - Obroc w lewo                 ");

	glColor3f(0.0f, 0.0f, 1.0f);
	printGL(0.0f, 7.8f, -1.0f, "ESC   - Wyjscie                    ");

}
