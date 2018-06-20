#ifndef __SCENE_3201
#define __SCENE_3201

#include <freeglut.h>
#include <list>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Renderable.h"
#include "Camera.h"

#define OBJ_STONE 0
#define OBJ_STARFISH 2
#define OBJ_FISH 3
#define OBJ_PLANT 4

#define SIGN(x) (x < 0 ? (-1) : 1)

class Scene
{
private:
	GLenum error;	
	GLenum polygonModel;	

	std::list<Renderable*> *elements;	
	std::list<Renderable*>::iterator iter;	

											
	static GLfloat ambient0[4];
	static GLfloat diffuse0[4];
	static GLfloat specular0[4];
	static GLfloat position0[4];


	static GLfloat ambient1[4];
	static GLfloat diffuse1[4];
	static GLfloat specular1[4];
	static GLfloat position1[4];
	static GLfloat direction1[4];

	static GLfloat spotAngle;

public:
	Camera camera;	
	int objects[5];	
	bool perspectiveMode;	
	bool showMenu;	
	bool light0On;	
	bool light1On;	
	bool fogMode;	
	bool lightMode;	

	static int width;	
	static int height;

public:
	Scene();	
	virtual ~Scene();	

	bool render(void);	
	void add(Renderable *object);	

private:
	void clear(void);	
	void printGL(GLfloat x, GLfloat y, GLfloat z, const char *str, int count);
	void printGL(GLfloat x, GLfloat y, GLfloat z, const char *str);
	void printMenu(void);	
	void drawHUD(void);	
};

#endif
