
#ifndef __RENDERABLE_3201
#define __RENDERABLE_3201

#include <freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>

#ifndef TEXTURE_IDS
#define TEXTURE_IDS
#define FLOOR_TEXTURE 1
#define FISH_TEXTURE 2
#endif

class Renderable
{
public:
	GLfloat x;	/// x obiektu
	GLfloat y;	/// y obiektu
	GLfloat z;	/// z obiektu

	GLfloat rx;	/// x kat obrotu obiektu
	GLfloat ry;	/// y kat obrotu obiektu
	GLfloat rz;	/// z kat obrotu obiektu

	GLfloat sx;	/// x skala obiektu
	GLfloat sy;	/// y skala obiektu
	GLfloat sz;	/// z skala obiektu

	bool isList;	

	static unsigned int textures[2];	
	static GLUquadricObj *quadric;	
public:
	Renderable();	
	virtual ~Renderable();	

	void build(GLuint &dlist);	
	void move(GLfloat x, GLfloat y, GLfloat z);	
	void rotate(GLfloat x, GLfloat y, GLfloat z);	
	void scale(GLfloat x, GLfloat y, GLfloat z);	
	void draw(void);

	static GLfloat getRand(GLfloat minimum, GLfloat range);
protected:
	virtual void _draw(void) = 0;	
	virtual void _draw_dlist(void) {}	
};
#endif

#ifndef __STARFISH_3201
#define __STARFISH_3201
class StarFish : public Renderable
{
private:
	static GLfloat vertex[];	
	static GLfloat normal[];	
	static GLfloat colours[];	
	static GLfloat material[4];
	static GLfloat shininess;
public:
	StarFish();	
	virtual ~StarFish();	

protected:
	void _draw(void);	
};
#endif

#ifndef __FISH_3201
#define __FISH_3201
class Fish : public Renderable
{
private:
	GLfloat tailAngle;
	GLfloat tailAngleCutOff;
	GLfloat tailAngleInc;
	static GLfloat vertex[];	
	static GLfloat normal[];	
	static GLfloat texels[]; 
	static GLfloat colours[];	
	static GLfloat material[4];
	static GLfloat shininess;
private:
	void drawSide(void);	
public:
	Fish();	
	virtual ~Fish();	

protected:
	void _draw(void);	
};
#endif

#ifndef __STONE_3201
#define __STONE_3201
class Stone : public Renderable
{
private:
	GLuint dlist;	
	static GLfloat material[4];
	static GLfloat shininess;
public:
	Stone();	
	virtual ~Stone();	
protected:
	void _draw(void);	
	void _draw_dlist(void);	

};
#endif

#ifndef __QUAD_3201
#define __QUAD_3201
class Quad : public Renderable
{
private:
	static GLfloat material[4];
	static GLfloat shininess;
public:
	Quad();	
	virtual ~Quad();	
protected:
	void _draw(void);	
};
#endif

#ifndef __PLANT_3201
#define __PLANT_3201
#include <list>
class Plant : public Renderable
{
private:
	GLuint dlist;	
	static GLfloat material1[4];
	static GLfloat material2[4];
	static GLfloat shininess;
public:
	Plant();	
	virtual ~Plant();	
protected:
	void _draw(void);	
	void generate(int level, int number);	
	void _draw_dlist(void);	
};
#endif
