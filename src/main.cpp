
#include <freeglut.h>
#include <iostream>

#include "Scene.h"
#include "Renderable.h"
#include "Textures.h"

#define OBJ_STONE 0
#define OBJ_STARFISH 2
#define OBJ_FISH 3
#define OBJ_PLANT 4

static Scene *scene;	/// scena
static bool wireMode = false;	
static bool flatShading = false;	

static bool init(int argc, char *argv[]);	
static void setupGL(void);	
static void animator(int type);	
static void resizeWindow(int w, int h);	
static void keyboardInput(unsigned char key, int x, int y);	
static void keyboardInput(int key, int x, int y);	
static void drawScene(void);	
static void addObject(int type);	
static void setupViewVolume(void);	
static void getTextures(void);	
static void getSandTexture(void);	
static void getFishTexture(void);	


using namespace std;

int main(int argc, char *argv[])
{
	cout << "-- Rozpoczynam program\n";

	srand(time(NULL));
	init(argc, argv);

	cout << "-- Rejestruje odwolania\n";
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(keyboardInput);

	
	cout << "-- Generuje i laduje tekstury\n";
	getTextures();

	// tworzenie sceny
	scene = new Scene();
	scene->perspectiveMode = true;

	// tworzenie dna
	Quad *quad;
	for (GLfloat i = -9.5; i <= 9.5; i++)
	{
		for (GLfloat j = -9.5; j <= 9.5; j++)
		{
			quad = new Quad();
			quad->ry = 0.0f;	
			quad->rx = 90.0f;
			quad->x = 3.5f * i;
			quad->z = 3.5f * j;
			quad->scale(3.5f, 3.5f, 1.0f);
			scene->add(quad);
		}
	}

	
	//oswietlenie, mgla
	scene->lightMode = !scene->lightMode;
	if (scene->lightMode) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);	

	scene->light0On = !scene->light0On;
	if (scene->light0On) glEnable(GL_LIGHT0);
	else glDisable(GL_LIGHT0);

	scene->light1On = !scene->light1On;
	if (scene->light1On) glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);

	scene->fogMode = !scene->fogMode;
	if (scene->fogMode) glEnable(GL_FOG);
	else glDisable(GL_FOG);

	// dodawanie obiektow
	for (int o = 0; o < 7; o++)
	{
		addObject(OBJ_STONE);
		addObject(OBJ_STONE);
		addObject(OBJ_STONE);
		addObject(OBJ_STONE);
		addObject(OBJ_STONE);

		addObject(OBJ_STARFISH);
		addObject(OBJ_FISH);
		addObject(OBJ_FISH);
		addObject(OBJ_FISH);
		addObject(OBJ_PLANT);
	}

	// start animacji (rozpoczecie "wiecznej" petli programu
	glutTimerFunc(25, animator, 0);
	glutMainLoop();

	return 0;
}


/// start animacji (generuje klatke co 25 milisekund)
void animator(int type)
{
	glutPostRedisplay();
	glutTimerFunc(25, animator, 0);
}


/// resizuje okno do full screena
void resizeWindow(int w, int h)
{
	glViewport(0, 0, w, h);

	scene->width = w;
	scene->height = h;

	setupViewVolume();
}


void keyboardInput(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:	
		exit(0);
		break;

	case ' ':	
		flatShading = !flatShading;
		if (flatShading) glShadeModel(GL_FLAT);
		else glShadeModel(GL_SMOOTH);
		break;

	case 'A':
	case 'a':
		scene->camera.tiltdown();
		break;

	case 'R':
	case 'r':
		scene->camera.reset();
		break;

	case 'Z':
	case 'z':
		scene->camera.tiltup();
		break;

	case 'W':
	case 'w':	
		wireMode = !wireMode;
		if (!wireMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_BLEND);
			glDisable(GL_LINE_SMOOTH);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_BLEND);
			glEnable(GL_LINE_SMOOTH);
		}
		break;
	}
}



void keyboardInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		scene->showMenu = !scene->showMenu;
		break;

	case GLUT_KEY_F2:
		addObject(OBJ_STONE);
		break;

	case GLUT_KEY_F4:
		addObject(OBJ_STARFISH);
		break;

	case GLUT_KEY_F5:
		addObject(OBJ_FISH);
		break;

	case GLUT_KEY_F6:
		addObject(OBJ_PLANT);
		break;

	case GLUT_KEY_LEFT:
		scene->camera.anticlockwise();
		break;

	case GLUT_KEY_RIGHT:
		scene->camera.clockwise();
		break;
	case GLUT_KEY_UP:
		scene->camera.inc();
		break;

	case GLUT_KEY_DOWN:
		scene->camera.dec();
		break;
	}
}




void drawScene()
{
	scene->render();
}



void addObject(int type)
{
	///generuje koordynaty wewnatrz kwadratu (dno) i dodaje tam obiekty

	GLfloat x = Renderable::getRand(-25.0f, 50.0f);
	GLfloat z = Renderable::getRand(-25.0f, 50.0f);


	GLfloat y;

	Renderable *object;

	switch (type)
	{
	case OBJ_STARFISH:
		y = -0.3f;
		object = new StarFish();
		break;
	case OBJ_STONE:
		y = -0.4f;
		object = new Stone();
		break;
	case OBJ_FISH:
		y = Renderable::getRand(-26.0f, 25.0f);
		object = new Fish();
		break;
	case OBJ_PLANT:
		y = 0.0f;
		object = new Plant();
		object->ry = 0.0f;
		break;
	}

	object->move(x, y, z);	// przesuniecie obiektu
	scene->add(object);	// dodanie obiektu do kolejki renderowania
	scene->objects[type]++;	// zwiekszenie licznika obiektow
}


void setupViewVolume(void)
{
	GLfloat aspect = (GLfloat)scene->width / (GLfloat)scene->height;
	GLfloat iaspect = (GLfloat)scene->height / (GLfloat)scene->width;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

		gluPerspective(-45.0f, aspect, 1.0f, 250.0f);


	glMatrixMode(GL_MODELVIEW);
}


/// inicjacja tekstur
void getTextures(void)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(2, Renderable::textures);

	getSandTexture();
	getFishTexture();
}


/// wczytanie tekstury piasku
void getSandTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, Renderable::textures[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, sand_image.pixel_data);
}


/// wczytanie tekstury ryb
void getFishTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, Renderable::textures[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, fish_image.pixel_data);
}



bool init(int argc, char *argv[])
{
	cout << "-- Uruchamianie GLUT\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	cout << "-- Tworzenie okna\n";
	glutCreateWindow("Aquarium Scene 3D");
	glutFullScreen();

	setupGL();

	return true;
}


/// konfiguracja efektow openGL

void setupGL(void)
{
	cout << "-- Konfiguracja OpenGL\n";

	// kolor tla
	glClearColor(0.251, 0.643, 0.875, 1.0);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// testowanie glebi
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// konfiguracja mgly
	glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	GLfloat fogColor[4] = { 0.0, 0.5, 0.55, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.01);
	glHint(GL_FOG_HINT, GL_NICEST);

	// normalizacja
	glEnable(GL_NORMALIZE);

	// konfiguracja oswietlenia
	glDisable(GL_LIGHTING);
	GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);



	// antialiasing
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(8.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// konfiguracja backface culling
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// korekcja perspektywy dla tekstur
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
