#include "Scene.h"

using namespace std;

int Scene::width;	
int Scene::height;	

					// kolory swiatla 0
GLfloat Scene::ambient0[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat Scene::diffuse0[4] = { 0.2, 0.2, 0.2, 1.0f };
GLfloat Scene::specular0[4] = { 0.259, 0.412, 0.549, 1.0f };
GLfloat Scene::position0[4] = { 0.0f, -1.0f, 0.0f, 0.0f };

// kolory swiatla 1
GLfloat Scene::ambient1[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat Scene::diffuse1[4] = { 0.45f, 0.45f, 0.45f, 1.0f };
GLfloat Scene::specular1[4] = { 0.5, 0.5, 0.5, 1.0f };
GLfloat Scene::position1[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat Scene::direction1[4] = { 0.0f, 0.0f, -1.0f };

// kat padania swiatla
GLfloat Scene::spotAngle = 15.f;


Scene::Scene()
{
	cout << "-- Tworzenie sceny\n";

	error = GL_NO_ERROR;
	polygonModel = GL_FILL;
	elements = new std::list<Renderable*>;	// tworzenie kolejki
	showMenu = true;	
	light0On = false;	
	light1On = true;	
	fogMode = false;	
	lightMode = false;	

						// liczniki = 0
	objects[OBJ_STONE] = 0;
	objects[OBJ_STARFISH] = 0;
	objects[OBJ_FISH] = 0;
	objects[OBJ_PLANT] = 0;

	// kolor swiatla 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

	// kolor swiatla 1
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
}


Scene::~Scene()
{
	cout << "++ Usuwanie sceny\n";
	delete elements;
}


/// generuje pojedyncza klatka
bool Scene::render(void)
{
	GLenum error;

	// kontrola bledow
	while ((error = glGetError()) != GL_NO_ERROR)
		cout << ">> Error: " << gluErrorString(error) << endl;

	
	clear();

	// swiatlo padajace z kamery
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);

	camera.position();

	// swiatlo kierunkowe
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	if (elements->size() > 0)
	{
		iter = elements->begin();
		while (iter != elements->end())
			(*iter++)->draw();
	}

	drawHUD();

	glutSwapBuffers();
	return true;
}


void Scene::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/// dodanie obiektu do kolejki rysowania

void Scene::add(Renderable *object)
{
	elements->push_back(object);
}


/// rysuje HUD
void Scene::drawHUD(void)
{
	glDisable(GL_DEPTH_TEST);

	// wylacza ie oswietlenia
	GLboolean lightsOn;
	glGetBooleanv(GL_LIGHTING, &lightsOn);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(-0.1f, 10.0f, 10.0f, -0.4f, -1.0f, 10.0f);

	// pobieranie aktualnej pozycji
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.259, 0.412, 0.549);

	printGL(0.0f, 0.0f, 0.0f, "Kamienie:    ", objects[OBJ_STONE]);
	printGL(1.5f, 0.0f, 0.0f, "Rozgwiazdy: ", objects[OBJ_STARFISH]);
	printGL(4.5f, 0.0f, 0.0f, "Ryby:     ", objects[OBJ_FISH]);
	printGL(6.0f, 0.0f, 0.0f, "Rosliny:   ", objects[OBJ_PLANT]);

	if (showMenu)
		printMenu();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

	if (lightsOn) glEnable(GL_LIGHTING);
}


void Scene::printGL(GLfloat x, GLfloat y, GLfloat z, const char *str, int count)
{
	char buffer[30];
	for (int i = 0; i < 30; ++i)
		buffer[i] = ' ';

	sprintf(buffer, "%s %i", str, count);
	printGL(x, y, z, buffer);
}


void Scene::printGL(GLfloat x, GLfloat y, GLfloat z, const char *str)
{
	glRasterPos3f(x, y, z);

	// print character one by one
	for (int j = 0; j < 30; j++) {
		int c = str[j];
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
	}
}


void Scene::printMenu(void)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	printGL(0.0f, 1.7f, -1.0f, "---[ Klawiszologia ]------------");

	glColor3f(0.0f, 1.0f, 0.0f);
	printGL(0.0f, 2.3f, -1.0f, "F1    - Wygeneruj kamien             ");
	printGL(0.0f, 2.6f, -1.0f, "F2    - Wygeneruj rozgwiazde          ");
	printGL(0.0f, 2.9f, -1.0f, "F3    - Wygeneruj rybe              ");
	printGL(0.0f, 3.2f, -1.0f, "F4    - Wygeneruj rosline             ");
	printGL(0.0f, 3.5f, -1.0f, "A/Z   - +/- Nachylenie               ");
	printGL(0.0f, 3.8f, -1.0f, "GORA  - Zbliz                         ");
	printGL(0.0f, 4.1f, -1.0f, "DOL   - Oddal                         ");
	printGL(0.0f, 4.3f, -1.0f, "LEWO  - Obroc w lewo                  ");
	printGL(0.0f, 4.6f, -1.0f, "PRAWO - Obroc w prawo                 ");
	printGL(0.0f, 4.9f, -1.0f, "R     - Resetuj kamere                ");


	glColor3f(0.0f, 0.0f, 1.0f);
	printGL(0.0f, 5.2f, -1.0f, "Underwater Scene by:             ");
	printGL(0.0f, 5.5f, -1.0f, "Przemyslaw Plociennik();         ");
	printGL(0.0f, 5.8f, -1.0f, "and Piotr Popowski();             ");

	glColor3f(0.0f, 0.0f, 0.0f);
	printGL(0.0f, 7.8f, -1.0f, "ESC   - Wyjscie                    ");

}
